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
 * Header files for Hitachi HD44780 LCD controller
 */

#ifndef _HD44780LCD_H_
#define _HD44780LCD_H_

/*
 * Main LCD Commands
 */
#define LCD_CLEAR               0x01    /* Clear Display */
#define LCD_CURSORHOME          0x02    /* Move cursor to HOME position.  Display unchanged */
#define LCD_INPUTSET            0x04    /* Cursor move direction */
#define LCD_DISPLAYCONTROL      0x08    /* Display, Cursor, blink on/off */
#define LCD_SHIFT               0x10    /* Remove cursor and whole display */
#define LCD_FUNCTIONSET         0x20    /* Set DL, Display, Line, Font */
#define LCD_SETCGRAMADDR        0x40    /* Set CGRAM Address */
#define LCD_SETDDRAMADDR        0x80    /* Set DDRAM Address */

/*
 * Display entry mode (OR/AND with LCD_INPUTSET)
 */
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/*
 * Display & Cursor control (OR/AND with LCD_DISPLAYCONROL)
 */
#define LCD_DISPLAYON           0x04	/* Turn Display on */
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02    /* Turn Cursor on */
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01    /* Set Cursor to blink */
#define LCD_BLINKOFF            0x00

/*
 * Display/Cursor shift (OR/AND with LCD_SHIFT)
 */
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

/*
 * Function set (OR/AND LCD_FUNCTIONSET)
 */
#define LCD_8BITMODE            0x10    /* 8-bit mode */
#define LCD_4BITMODE            0x00    /* 4-bit mode */
#define LCD_2LINE               0x08    /* Two line mode */
#define LCD_1LINE               0x00    /* One line mode */
#define LCD_5x10DOTS            0x04	/* 5x10 dot Font Style */
#define LCD_5x8DOTS             0x00    /* 5x8 dot Font Style */

#endif
