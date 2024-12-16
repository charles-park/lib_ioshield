//-----------------------------------------------------------------------------
/**
 * @file ioshield.c
 * @author charles-park (charles.park@hardkernel.com)
 * @brief LCD 16x2 I/O Shield control library (wiringPi)
 * @version 0.1
 * @date 2024-12-13
 *
 * @copyright Copyright (c) 2022
 *
 */
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>

//-----------------------------------------------------------------------------
// for WiringPi
// apt install odroid-wiringpi libwiringpi-dev (wiringpi package)
//-----------------------------------------------------------------------------
#include "lib_ioshield.h"

//-----------------------------------------------------------------------------
struct ioshield {
    int  fd;
    char fb[BOARD_LCD_ROW][BOARD_LCD_COL+1];

    // call back func
    int (*callback_func)(int);
};

struct ioshield shield;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
pthread_t thread_fb;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *thread_fb_func (void *arg)
{
    struct ioshield *ps = (struct ioshield *)arg;
    int row, col;

    while (1) {
        for (row = 0; row < BOARD_LCD_ROW; row++) {
            pthread_mutex_lock (&mutex);
            lcdPosition(ps->fd, 0, row);
            for (col = 0; col < BOARD_LCD_COL; col++) {
                lcdPutchar (ps->fd, ps->fb[row][col]);
            }
            pthread_mutex_unlock (&mutex);
        }
        usleep (100 * 1000);
    }
    return arg;
}

//------------------------------------------------------------------------------
pthread_t thread_bt;

static void *thread_bt_func (void *arg)
{
    struct ioshield *ps = (struct ioshield *)arg;
    int bt1_old, bt2_old;

    bt1_old = digitalRead (PORT_BUTTON1);
    bt2_old = digitalRead (PORT_BUTTON2);

    while (1) {
        if (ps->callback_func != NULL) {
            if (bt1_old != digitalRead(PORT_BUTTON1)) {
                bt1_old  = digitalRead(PORT_BUTTON1);
                ps->callback_func (bt1_old ? eBT1_RELEASE : eBT1_PRESS);
            }
            if (bt2_old != digitalRead(PORT_BUTTON2)) {
                bt2_old  = digitalRead(PORT_BUTTON2);
                ps->callback_func (bt2_old ? eBT2_RELEASE : eBT2_PRESS);
            }
        }
        usleep (100 * 1000);
    }
    return arg;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int ioshield_led_set (int led_num, int led_state)
{
    int led_port;

    switch (led_num) {
        case 1: led_port = PORT_LED1;   break;
        case 2: led_port = PORT_LED2;   break;
        case 3: led_port = PORT_LED3;   break;
        case 4: led_port = PORT_LED4;   break;
        case 5: led_port = PORT_LED5;   break;
        case 6: led_port = PORT_LED6;   break;
        case 7: led_port = PORT_LED7;   break;
        default :                       return 0;
    }

    digitalWrite (led_port, led_state ? 1 : 0);
    return 1;
}

//------------------------------------------------------------------------------
int ioshield_lcd_clear  (int line)
{
    if (shield.fd < 0)  return 0;

    switch (line) {
        case 0 ... 1:
            memset (&shield.fb [line][0], ' ', BOARD_LCD_COL);
            break;
        default :
            memset (&shield.fb [0][0], ' ', BOARD_LCD_COL);
            memset (&shield.fb [1][0], ' ', BOARD_LCD_COL);
            break;
    }
    return 1;
}

//------------------------------------------------------------------------------
int ioshield_lcd_printf (int x, int y, char *fmt, ...)
{
    char buf[BOARD_LCD_COL * 2], len;
    va_list va;

    if (shield.fd < 0)  return 0;

    memset(buf, 0, sizeof(buf));

    va_start(va, fmt);
    len = vsprintf(buf, fmt, va);
    va_end(va);

    if ((y == 0) || (y == 1)) {
        memcpy (&shield.fb [y][x], buf,
                len < BOARD_LCD_COL ? len : BOARD_LCD_COL);
    }

    return 1;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int ioshield_init (int (*bt_callback_func)(int))
{
    wiringPiSetup();

    memset (&shield, 0, sizeof(struct ioshield));

    shield.fd = lcdInit (BOARD_LCD_ROW, BOARD_LCD_COL, BOARD_LCD_BUS,
                        PORT_LCD_RS, PORT_LCD_E,
                        PORT_LCD_D4, PORT_LCD_D5, PORT_LCD_D6, PORT_LCD_D7,
                        0, 0, 0, 0);
    if (shield.fd < 0) {
        fprintf (stderr, "%s : lcd init failed!\n", __func__);
        return 0;
    }
    // thread lcd
    pthread_create (&thread_fb, NULL, thread_fb_func, (void *)&shield);

    // Button Pull Up Enable.
    pinMode (PORT_BUTTON1, INPUT);
    pullUpDnControl (PORT_BUTTON1, PUD_UP);

    pinMode (PORT_BUTTON2, INPUT);
    pullUpDnControl (PORT_BUTTON2, PUD_UP);

    pinMode (PORT_LED1, OUTPUT);    digitalWrite(PORT_LED1, 0);
    pinMode (PORT_LED2, OUTPUT);    digitalWrite(PORT_LED2, 0);
    pinMode (PORT_LED3, OUTPUT);    digitalWrite(PORT_LED3, 0);
    pinMode (PORT_LED4, OUTPUT);    digitalWrite(PORT_LED4, 0);
    pinMode (PORT_LED5, OUTPUT);    digitalWrite(PORT_LED5, 0);
    pinMode (PORT_LED6, OUTPUT);    digitalWrite(PORT_LED6, 0);
    pinMode (PORT_LED7, OUTPUT);    digitalWrite(PORT_LED7, 0);

    // thread bt
    shield.callback_func = bt_callback_func;
    pthread_create (&thread_bt, NULL, thread_bt_func, (void *)&shield);

    return 1;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------