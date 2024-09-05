/*
 * main.c:
 * Copyright (c) 2014-2020 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "nvs_flash.h"
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
        if(xQueueReceive(uart0_queue, (void *)&event, portMAX_DELAY))
        {
            bzero(buf_data, BUF_SIZE);
            uart_read_bytes(UART_NUM_0, buf_data, event.size, portMAX_DELAY);
            if(rtrobot_sock_status==true)
                send(rtrobot_sock, buf_data, event.size, 0);
        }
    }
}

void rtrobot_uart1_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    //Install UART driver, and get the queue.
    uart_driver_install(UART_NUM_0, BUF_SIZE, BUF_SIZE, 20, &uart0_queue);
}


void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialise_wifi();
    wait_for_ip();
    rtrobot_uart1_init();

    xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
    xTaskCreate(uart1_event_task, "uart1_event_task", 4096 *2, NULL, 2, NULL);
}
