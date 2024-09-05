/*
 * main.c:
 * Copyright (c) 2014-2020 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 */

#include <stdio.h>
#include <nvs_flash.h>
#include <tcpip_adapter.h>
#include <esp_event.h>
#include <driver/uart.h>
#include <string.h>
#include <esp_system.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "tcp_server.h"

#define BUF_SIZE    1024
static QueueHandle_t uart0_queue;

void uart1_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t* buf_data = (uint8_t*) malloc(BUF_SIZE);

    while (1)
    {
        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, &event, pdMS_TO_TICKS(200)))
        {
            int len=0;
            bzero(buf_data, BUF_SIZE);
            len=uart_read_bytes(UART_NUM_0, buf_data, BUF_SIZE, pdMS_TO_TICKS(200));
            if(rtrobot_sock_status==true)
                send(rtrobot_sock, buf_data, len, 0);
        }
    }
}

void rtrobot_uart1_init(void)
{
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    //Set UART pins (using UART default pins ie no changes.)
    uart_set_pin(UART_NUM_0, UART_NUM_0_TXD_DIRECT_GPIO_NUM, UART_NUM_0_RXD_DIRECT_GPIO_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    //Install UART driver, and get the queue.
    uart_driver_install(UART_NUM_0, BUF_SIZE, BUF_SIZE, 20, &uart0_queue, 0);
}

void app_main()
{
    nvs_flash_init();
    tcpip_adapter_init();
    esp_event_loop_create_default();
    wlan_connect();


    rtrobot_uart1_init();
    xTaskCreate(uart1_event_task, "uart1_event_task", 4096 *2, NULL, 2, NULL);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
}
