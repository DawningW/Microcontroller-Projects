#include <stdio.h>

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_spiffs.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "wifi.h"
#include "bluetooth.h"
#include "http_server.h"

#include "lwip/api.h"
#include "mdns.h"
#include "websocket_server.h"

#define NAME "Mini Chat Server"
#define VERSION "V1.0"

static const char TAG[] = "MCS";

static QueueHandle_t client_queue;

void start_mdns_service()
{
    esp_err_t err = mdns_init();
    if (err) {
        printf("MDNS init failed: %d\n", err);
        return;
    }
    mdns_hostname_set("esp32");
    mdns_instance_name_set("My ESP32 Thing");
    mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0);
    mdns_service_add(NULL, "_ws", "_tcp", 81, NULL, 0);
}

void websocket_callback(uint8_t num, WEBSOCKET_TYPE_t type, char *msg, uint64_t len) {
    switch (type) {
        case WEBSOCKET_CONNECT:
            ESP_LOGI(TAG, "Client %i connected!", num);
            break;
        case WEBSOCKET_DISCONNECT_EXTERNAL:
            ESP_LOGI(TAG, "Client %i disconnected!", num);
            break;
        case WEBSOCKET_DISCONNECT_INTERNAL:
            ESP_LOGI(TAG,"Client %i was disconnected", num);
            break;
        case WEBSOCKET_DISCONNECT_ERROR:
            ESP_LOGI(TAG, "Client %i was disconnected due to an error", num);
            break;
        case WEBSOCKET_TEXT:
            if (len) {
                ESP_LOGI(TAG, "Received message from %i: %s", num, msg);
                bluetooth_send(msg);
            }
            break;
        case WEBSOCKET_BIN:
            ESP_LOGI(TAG, "Client %i sent binary message of size %i: %s", num, (uint32_t) len, msg);
            break;
        case WEBSOCKET_PING:
            ESP_LOGI(TAG, "Client %i pinged us with message of size %i: %s", num, (uint32_t) len, msg);
            break;
        case WEBSOCKET_PONG:
            ESP_LOGI(TAG, "Client %i responded to the ping", num);
            break;
    }
}

static void server_task(void *pvParameters) {
    struct netconn *conn, *newconn;
    static err_t err;
    client_queue = xQueueCreate(10, sizeof(struct netconn*));
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 81);
    netconn_listen(conn);
    ESP_LOGI(TAG, "WebSocket server listening");
    do {
        err = netconn_accept(conn, &newconn);
        if (err == ERR_OK) {
            xQueueSendToBack(client_queue, &newconn, portMAX_DELAY);
        }
    } while(err == ERR_OK);
    netconn_close(conn);
    netconn_delete(conn);
    vTaskDelete(NULL);
}

static void server_handle_task(void *pvParameters) {
    struct netconn* conn;
    ESP_LOGI(TAG, "Start WebSocket server handler");
    for(;;) {
        xQueueReceive(client_queue, &conn, portMAX_DELAY);
        if (!conn) continue;
        netconn_set_recvtimeout(conn, 1000);
        static err_t err;
        struct netbuf* inbuf;
        static char* buf;
        static uint16_t buflen;
        err = netconn_recv(conn, &inbuf);
        netbuf_data(inbuf, (void**) &buf, &buflen);
        if(buf) {
            ws_server_add_client(conn, buf, buflen, "/", websocket_callback);
            netbuf_delete(inbuf);
        }
    }
    vTaskDelete(NULL);
}

void print_sysinfo() {
    ESP_LOGI(TAG, "Collecting system info");
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("%s chip with %d cores, WiFi%s%s, silicon revision %d.\n",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "",
            chip_info.revision);
    printf("%s flash: %dMB\n",
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "Embedded" : "External",
            spi_flash_get_chip_size() / (1024 * 1024));
    printf("Free heap: %d\n", esp_get_free_heap_size());
}

void app_main() {
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize SPIFFS
    esp_vfs_spiffs_conf_t spiffs_conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&spiffs_conf));
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // Initialize bluetooth
    bluetooth_init();

    // Initialize wifi
    wifi_init();
    // Start SoftAP
    wifi_init_ap();
    // Start STA
    wifi_init_sta("****", "********");
    ESP_LOGE(TAG, "目前需要你手动填写Wifi SSID和密码");

    // Initialize mDNS
    start_mdns_service();
    // Initialize HTTP Server
    webserver_init();
    // Initialize WebSocket Server
    ws_server_start();
    xTaskCreate(&server_task, "server_task", 3000, NULL, 9, NULL);
    xTaskCreate(&server_handle_task, "server_handle_task", 4000, NULL, 6, NULL);

    // debug
    print_sysinfo();
    fflush(stdout);

    // test
    char received[50];
    while (true) {
        if (bluetooth_receive(received)) {
            received[49] = '\0';
            printf("Received message from Bluetooth: %s\n", received);
            ws_server_send_text_all(received, strlen(received));
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
