# I2C

A sample application to control a Grove LCD RGB Backlight 2.0 device over
I2C.

The Grove LCD RGB Backlight 2.0 is attached to a Grove breakout board which
is plugged into the Arduino header of the Galileo Gen2.  It exposes 2 I2C
devices, the LCD display and the backlight controller.

A library of common LCD and backlight operatons is supplied as part of the
demo.  The main program uses the library which in turn uses I2C to send
commands to the 2 I2C controllers.

main.c -> lcdutils.c -> I2C operations

## Sample Output

```
Process started (ID: 0x1019010)

```
