#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "esp_http_server.h"

void webserver_init();
httpd_handle_t webserver_start();
void webserver_stop(httpd_handle_t server);

#endif
