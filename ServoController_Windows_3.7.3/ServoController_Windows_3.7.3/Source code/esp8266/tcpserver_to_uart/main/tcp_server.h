/*
 * tcp_server.h:
 * Copyright (c) 2014-2020 Rtrobot. <admin@rtrobot.org>
 *  <http://rtrobot.org>
 ***********************************************************************
 */

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string.h>
#include <sys/param.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include <driver/uart.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>



#define CONFIG_WLAN_SSID "myssid"
#define CONFIG_WLAN_PASSWORD "mypassword"
#define CONFIG_TCP_SERVER_PORT 5555


extern int rtrobot_sock;
extern bool rtrobot_sock_status;

void initialise_wifi(void);
void wait_for_ip(void);
void tcp_server_task(void *pvParameters);

#endif //TCP_SERVER_H
