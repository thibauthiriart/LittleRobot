/*
 * main.c:
 * Copyright (c) 2014-2021 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

int main()
{
    // Set up our UART with the required speed.
    uart_init(uart0, 9600);

    gpio_set_function(PICO_DEFAULT_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(PICO_DEFAULT_UART_RX_PIN, GPIO_FUNC_UART);


    uart_puts(uart0, "#3P500T2000\r\n");
    sleep_ms(2000);

    uart_puts(uart0, "#2P1723#3P1500T2000\r\n");
    sleep_ms(2000);

    uart_puts(uart0, "#3P2500T2000D1555\r\n");
    sleep_ms(1555);

    uart_puts(uart0, "#3P1500T2000\r\n");
    sleep_ms(2000);

    //Test for send wrong instruction
    uart_puts(uart0, "#3P2500T9000D9000\r\n");

    //Stop wrong instruction
    uart_puts(uart0, "~ST");

    //Change right instruction
    uart_puts(uart0, "#2P2500T9000D9000\r\n");
    sleep_ms(9000);

    //Test for action groups,Group 1 run 3 times
    uart_puts(uart0, "G1F3\r\n");

    while(1);
}
