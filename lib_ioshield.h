//-----------------------------------------------------------------------------
/**
 * @file ioshield.h
 * @author charles-park (charles.park@hardkernel.com)
 * @brief LCD 16x2 I/O Shield control library
 * @version 0.1
 * @date 2024-12-13
 *
 * @copyright Copyright (c) 2022
 *
 */
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef __IOSHIELD_H__
#define __IOSHIELD_H__

//------------------------------------------------------------------------------
#include <wiringPi.h>
#include <lcd.h>

//------------------------------------------------------------------------------
//
// for lcd interface data
//
//------------------------------------------------------------------------------
#define BOARD_LCD_ROW   2   // 16 Char
#define BOARD_LCD_COL   16  // 2 Line
#define BOARD_LCD_BUS   4   // Interface 4 Bit mode

//------------------------------------------------------------------------------
#define PORT_LCD_RS     7
#define PORT_LCD_E      0
#define PORT_LCD_D4     2
#define PORT_LCD_D5     3
#define PORT_LCD_D6     1
#define PORT_LCD_D7     4

//------------------------------------------------------------------------------
//
// Button:
//
//------------------------------------------------------------------------------
#define PORT_BUTTON1	5
#define PORT_BUTTON2	6

enum {
    eBT_UNKNOWN,
    eBT1_PRESS,
    eBT1_RELEASE,
    eBT2_PRESS,
    eBT2_RELEASE,
    eBT_END
};

//------------------------------------------------------------------------------
//
// LED:
//
//------------------------------------------------------------------------------
// PCB Layout
// | D1 | D2 | D3 | D4 | D7 | D6 | D5 |
//------------------------------------------------------------------------------
#define PORT_LED1       21
#define PORT_LED2       22
#define PORT_LED3       23
#define PORT_LED4       24
#define PORT_LED7       11
#define PORT_LED6       26
#define PORT_LED5       27

//------------------------------------------------------------------------------
//
// SPI:
//
//------------------------------------------------------------------------------
#define PORT_MISO       13
#define PORT_MOSI       12
#define PORT_SCLK       14
#define PORT_CE0        10

//------------------------------------------------------------------------------
//
// I2C: I2C-1, I2C-2
//
//------------------------------------------------------------------------------
#define PORT_SDA1       3
#define PORT_SCL1       5
#define PORT_SDA2       27
#define PORT_SCL2       28

//------------------------------------------------------------------------------
// Function prototype
//------------------------------------------------------------------------------
extern int ioshield_led_byte    (unsigned char led_byte);
extern int ioshield_led_set     (int led_port, int led_state);
extern int ioshield_lcd_clear   (int line);
extern int ioshield_lcd_printf  (int x, int y, char *fmt, ...);
extern int ioshield_init        (int (*bt_callback_func)(int));

//------------------------------------------------------------------------------
#endif  //  #define __IOSHIELD_H__

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
