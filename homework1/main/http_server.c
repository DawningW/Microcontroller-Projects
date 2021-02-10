#include "http_server.h"

#include <stdio.h>
#include <string.h>

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

static const char TAG[] = "HTTP";

static char index_page[2048];
static size_t str_len;
static httpd_handle_t webServer = NULL;

static void connect_handler(void* arg, esp_event_base_t event_base, 
                            int32_t event_id, void* event_data) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting http server");
        *server = webserver_start();
    }
}

static void disconnect_handler(void* arg, esp_event_base_t event_base, 
                               int32_t event_id, void* event_data) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping http server");
        webserver_stop(*server);
        *server = NULL;
    }
}

static esp_err_t index_get_handler(httpd_req_t *req) {
    httpd_resp_send(req, index_page, str_len);
    return ESP_OK;
}

static const httpd_uri_t url_index = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = index_get_handler,
    .user_ctx = "Index"
};

void webserver_init() {
    FILE *file = fopen("/spiffs/index.html", "r");
    if (file != NULL) {
        char *pc = index_page;
        int ch;
        while ((ch = fgetc(file)) != EOF) {
            *(pc++) = ch;
        }
        str_len = strlen(index_page);
        fclose(file);
        puts(index_page);
        putchar('\n');
    } else {
        ESP_LOGE(TAG, "Failed to open file for reading");
        strcpy(index_page, "index.html Not Found");
    }
    ESP_LOGI(TAG, "Initialize http server");
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &webServer));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &webServer));
    webServer = webserver_start();
}

httpd_handle_t webserver_start() {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    ESP_LOGI(TAG, "Starting server on port: %d", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &url_index);
        return server;
    }
    ESP_LOGE(TAG, "Error starting http server!");
    return NULL;
}

void webserver_stop(httpd_handle_t server) {
    httpd_stop(server);
}
