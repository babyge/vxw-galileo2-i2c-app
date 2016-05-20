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
 * A sample application to control a Grove LCD RGB Backlight 2.0 device over
 * I2C.
 *
 * The Grove LCD RGB Backlight 2.0 is attached to a Grove breakout board which
 * is plugged into the Arduino header of the Galileo Gen2.  It exposes 2 I2C
 * devices, the LCD display and the backlight controller.
 *
 * A library of common LCD and backlight operatons is supplied as part of the
 * demo.  The main program uses the library which in turn uses I2C to send
 * commands to the 2 I2C controllers.
 *
 * main.c -> lcdutils.c -> I2C operations
 *
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
#include <linux/i2c-dev.h>
#else
#include "i2c.h"

extern void usleep (long microseconds);

#endif


#include "hd44780Lcd.h"
#include "lcdutils.h"

#define LCD_MESSAGE "WRS Helix Rocks!"

/*******************************************************************************
* main - User application entry function
*
* This the the entry point for a VxWorks User Mode application
*/

int main(int argc, char *argv[])
{

    int i;
    unsigned char buffer[16];
    struct tm *lcl_time;
    time_t currtime;

    /* Initialize the LCD utility layer */
    lcd_open();

    /* Set the LCD to 2 line mode */
    lcd_command (LCD_FUNCTIONSET | LCD_2LINE);

    /* Turn on the display. No Cursor. No Blink */
    lcd_command (LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF);

    /* Clear the display */
    lcd_command(LCD_CLEAR);
    usleep(500000);

    /* Put the cursor at Row 0, Column 0 */
    lcd_command(0x80);

    /* Write a message on the first line (16 characters are visible) */
    lcd_write((unsigned char *)LCD_MESSAGE, strlen(LCD_MESSAGE));
    usleep(500000);

    /* Set red backlight */
    lcd_set_color(255,0,0);
    usleep(1000000);

    /* Set green backlight */
    lcd_set_color(0, 255, 0);
    usleep(1000000);

    /* Set blue backlight */
    lcd_set_color(0, 0, 255);
    usleep(1000000);

    /* Set white backlight */
    lcd_set_color(255, 255, 255);

    /* Toggle the display off then on */
    lcd_command (LCD_DISPLAYCONTROL | LCD_BLINKOFF);
    usleep(500000);
    lcd_command (LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKOFF);
    usleep(500000);

    /* Put the cursor at Row 1, Column 0 */
    lcd_command(0xc0);

    /* Turn the curson on */
    lcd_command (LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF);
    usleep(500000);

    /* Make the cursor blink */
    lcd_command (LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON);
    usleep(500000);

    /* Turn the blink off */
    lcd_command (LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF);
    usleep(500000);

    /* Turn the cursor off */
    lcd_command (LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    usleep(500000);

    /* Scroll the display left */
    for (i=0; i<5; i++)
            {
            lcd_command(LCD_SHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
            usleep(250000);
            }

    /* Scroll the display right */
    for (i=0; i<5; i++)
            {
            lcd_command(LCD_SHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
            usleep(250000);
            }

   /* Go into continuous update mode */
  i=0;
  while (1)
       {
       /* Put the cursor at Row 1, Column 0 */
       lcd_command(0xc0);

       /* Get the current system time and format it for display */
       currtime = time (NULL);
       lcl_time = localtime(&currtime);

       sprintf((char *)buffer, "Time: %02d:%02d:%02d", lcl_time->tm_hour,
           lcl_time->tm_min, lcl_time->tm_sec);

       /* Write the current system time */
       lcd_write(buffer, strlen((char *)buffer));

       /* Sleep for 5 seconds */
       usleep(500000);
       }

    /* All done, clean up */
    lcd_close();



}
