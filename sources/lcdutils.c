/*
* Copyright (c) 2016, Wind River Systems, Inc.
*
* Redistribution and use in source and binary forms, with or without modification, are
* permitted provided that the following conditions are met:
*
* 1) Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2) Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* 3) Neither the name of Wind River Systems nor the names of its contributors may be
* used to endorse or promote products derived from this software without specific
* prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * DESCRIPTION
 * A sample utility library to program the Grove LCD RGB Backlight 2.0 device
 * over I2C.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#ifdef LINUX
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#else
#include <ioLib.h>
#include <i2c.h>
#define I2C_SLAVE I2C_SLAVE_ADDR

extern void usleep (long microseconds);

#endif


#include "hd44780Lcd.h"
#include "lcdutils.h"

static int lcd_fd = -1;
static int rgb_fd = -1;

/*******************************************************************************
* lcd_err_exit - Called to clean up and exit with an error
*
* Close the LCD and RGB I2C device file descriptor, print the supplied message
* along with the current value of errno, and exit with error (-1)
*
* Arguments: message - String pointer indicating something about the error
*
* Returns:  Never.  Program exits
*
*/
static void lcd_err_exit(char *message)
{
    (void)close(lcd_fd);
    (void)close(rgb_fd);

    fprintf(stderr,"%s. Errno = %d\n", message, errno);

    exit(-1);
}

/*******************************************************************************
* lcd_open - Opens file descriptors for the LCD and RGB I2C devices
*
* Open the I2C bus and set a FD to device 0x3e. (LCD controller)
* Open the I2C bus and set another FD to device 0x60 (RGB controller)
* *
* Arguments: None
*
* Returns:  0 - Success
*           Exits on error via lcd_err_exit()
*/
int lcd_open()
{

   /* Create the LCD display file descriptor. Device 0x3e  */
    lcd_fd = open("/dev/i2c-0", O_RDWR);
    if (lcd_fd == -1)
        lcd_err_exit("Error opening I2C Bus");


    if (ioctl(lcd_fd, I2C_SLAVE, 0x3e) < 0)
        lcd_err_exit("Error setting LCD I2C address");

    /* Create the LCD RGB Backlight file descriptor. Device 0x62 */
    rgb_fd = open("/dev/i2c-0", O_RDWR);
    if (rgb_fd == -1)
        lcd_err_exit("Error opening I2C Bus");


    if (ioctl(rgb_fd, I2C_SLAVE, 0x62) < 0)
        lcd_err_exit("Error settng RGB I2C address");

    return(0);
}

/*******************************************************************************
* lcd_close - Close the LCD and RBG file descriptors

* Arguments: None
*
* Returns:  void
*
*/
void lcd_close()
{
    (void)close(lcd_fd);
    (void)close(rgb_fd);
}

/*******************************************************************************
* lcd_command - Send a 1 byte command to the LCD controller
*
* Create a I2C packet with byte 0 set to 0 (Command byte) and byte 1 set to
* the value passed by the user
*
* Arguments: value - 1 byte command (See HD44780 docs)
*
* Returns:  void
*           Exits on error via lcd_err_exit()
*/
void lcd_command(unsigned char value)
{
    unsigned char data[2];

    data[0] = 0x00;     // Control byte, (Do not set MSB as indicated in docs)
    data[1] = value;    // Data byte

    if (write(lcd_fd, data, 2) != 2)
        lcd_err_exit("Error sending LCD command");

    // Wait long enough for command to work.  See GroveLCD documents
    switch(value)
        {
        case LCD_CLEAR:        // Wait 1.53 ms (rounded to 1.6 ms)
        case LCD_CURSORHOME:
            usleep(1600);
            break;

        default:              // Wait 39 usec (rounded up to 40)
            usleep(40);
            break;
        }

}


/*******************************************************************************
* lcd_write - Send a multibyte string to the Display RAM on LCD controller
*
* Create a I2C packet with byte 0 set to 0x40 (DPRAM) and the rest of the
* callers buffer after that.  Send it all off to the LCD I2C controller.
*
* Arguments: value - array of DPRAM values (See HD44780 docs)
*            len - Number of bytes in value[]
*
* Returns:  void
*           Exits on error via lcd_err_exit()
*/
void lcd_write(unsigned char *value, int len)
{
    unsigned char data[128] = {0};
    int i;
    int wr_len;

    if (len > 127)
        return;

    data[0] = 0x40;             // RS signal set

#ifdef LINUX   // Linux has issues writing large i2C messages.  So we
               // just do one character at a time.
    for (i=0; i<len; i++)      // Add data values to the stream
        {
        data[1] = value[i];
        if (write(lcd_fd, data, 2) != 2)    // Write the character
            lcd_err_exit("Error sending LCD command.");
        usleep(45);         // Wait 43 usec (rounded up)
        }
#else

    for (i=1; i<=len; i++)
        data[i] = value[i-1];

    wr_len = i;
    if (write(lcd_fd, data, wr_len) != wr_len) // Write the whole thing
        lcd_err_exit("Error sending LCD command");
    usleep(45);
#endif

}

/*******************************************************************************
* lcd_set_color - Set the backlight color to the R-G-B value specified
*
* Perform the series of writes to cause the Phillips RGB backlight controller
* to set the desired color.
*
* Arguments: red -   Red level (0x00-0xff)
*            green - Green lever (0x00-0xff)
*            blue  - Blue level (0x00-0xff)
*
* Returns:  void
*           Exits on error via lcd_err_exit()
*/
void lcd_set_color(unsigned char red, unsigned char green, unsigned char blue)
{
   char data[2];


   data[0] = 0;
   data[1] = 0;
   if (write(rgb_fd, data, 2) != 2)    // Write the character
        lcd_err_exit("Error sending command to RBG controller");

    data[0] = 0;
    data[1] = 1;
    if (write(rgb_fd, data, 2) != 2)    // Write the character
       lcd_err_exit("Error sending command to RBG controller");

    data[0] = 8;
    data[1] = 0xaa;
    if (write(rgb_fd, data, 2) != 2)    // Write the character
       lcd_err_exit("Error sending command to RBG controller");

   data[0] = 4;
   data[1] = red;
   if (write(rgb_fd, data, 2) != 2)    // Write the character
        lcd_err_exit("Error settingRBG controller red level");

    data[0] = 3;
    data[1] = green;
    if (write(rgb_fd, data, 2) != 2)    // Write the character
         lcd_err_exit("Error setting RBG controller green level") ;

    data[0] = 2;
    data[1] = blue;
    if (write(rgb_fd, data, 2) != 2)    // Write the character
        lcd_err_exit("Error setting RBG controller blue level");


}
