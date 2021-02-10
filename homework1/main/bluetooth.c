#include "bluetooth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "driver/uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

// Message APP
#define PROFILE_APP_ID 0
#define PROFILE_NUM 1

// Message Service
#define SERVICE_CLASS_UUID 0xABCD
#define SERVICE_UUID 0x00FF
#define CHAR_RECEIVE_UUID 0xFF01
#define CHAR_SEND_UUID 0xFF02
#define HANDLE_NUM 6

static const char TAG[] = "MCS-ble";

// TODO 有时间再详读GAP的协议吧= =我快吐了
static uint8_t adv_data_raw[] = {
    /* Flags */
    0x02, 0x01, 0x06,
    /* Complete List of 16-bit Service Class UUIDs */
    0x03, 0x03, 0xAB, 0xCD,
    /* Complete Local Name in advertising */
    0x0F, 0x09, 'E', 'S', 'P', '_', '3', '2', '_', 'W', 'C', 'T', 'E', 'S', 'T'
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
};

static struct gatts_profile_instance profile_tab[PROFILE_NUM];

static bool is_connected = false;
static bool can_notify = false;
static esp_bd_addr_t remote_bda = {0x0};

// static xQueueHandle message_queue = NULL;
//static uint8_t tx_buffer[20];
static char buffer[512];
/*
static esp_attr_value_t char_value = {
    .attr_max_len = 0xB0,
    .attr_len = sizeof(tx_buffer),
    .attr_value = tx_buffer,
};
*/

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
        case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            esp_ble_gap_start_advertising(&adv_params);
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            ESP_ERROR_CHECK(param->adv_start_cmpl.status);
            break;
        default:
            break;
    }
}

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    if (event == ESP_GATTS_REG_EVT) {
        if (param->reg.status == ESP_GATT_OK) {
            profile_tab[param->reg.app_id].gatts_if = gatts_if;
        } else {
            ESP_LOGE(TAG, "Register app %04x failed: %d\n", param->reg.app_id, param->reg.status);
            return;
        }
    }
    for (int idx = 0; idx < PROFILE_NUM; ++idx) {
        if (gatts_if == ESP_GATT_IF_NONE || gatts_if == profile_tab[idx].gatts_if) {
            if (profile_tab[idx].gatts_cb) {
                profile_tab[idx].gatts_cb(event, gatts_if, param);
            }
        }
    }
}

// 用esp_ble_gatts_get_attr_value(profile_tab[PROFILE_APP_ID].char_handle[], &length, &char);获取特征值
void gatts_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
        case ESP_GATTS_REG_EVT: {
            // 应用注册成功, 创建服务
            ESP_LOGI(TAG, "REGISTER_APP_EVT, status: %d, app_id: %d", param->reg.status, param->reg.app_id);
            profile_tab[PROFILE_APP_ID].service_id.is_primary = true;
            profile_tab[PROFILE_APP_ID].service_id.id.inst_id = 0x00;
            profile_tab[PROFILE_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
            profile_tab[PROFILE_APP_ID].service_id.id.uuid.uuid.uuid16 = SERVICE_UUID;
            esp_ble_gatts_create_service(gatts_if, &profile_tab[PROFILE_APP_ID].service_id, HANDLE_NUM);
            break;
        }
        case ESP_GATTS_READ_EVT: {
            // 客户端请求读取特征或描述符
            ESP_LOGI(TAG, "GATT_READ_EVT, conn_id: %d, trans_id: %d, handle: %d", param->read.conn_id, param->read.trans_id, param->read.handle);
            if (param->read.handle == profile_tab[PROFILE_APP_ID].descr_handle[0]) {
                // CCCD
                esp_gatt_rsp_t rsp;
                memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
                rsp.attr_value.handle = param->read.handle;
                rsp.attr_value.len = 2;
                rsp.attr_value.value[0] = can_notify;
                rsp.attr_value.value[1] = 0x00;
                esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
            }
            break;
        }
        case ESP_GATTS_WRITE_EVT: {
            // 客户端请求写特征或描述符
            ESP_LOGI(TAG, "GATT_WRITE_EVT, conn_id: %d, trans_id: %d, handle: %d", param->write.conn_id, param->write.trans_id, param->write.handle);
            if (!param->write.is_prep) {
                ESP_LOGI(TAG, "GATT_WRITE_EVT, value len %d, value:", param->write.len);
                ESP_LOG_BUFFER_HEX(TAG, param->write.value, param->write.len);
                if (param->write.handle == profile_tab[PROFILE_APP_ID].char_handle[0]) {
                    // TODO 接收数据
                    memcpy(buffer, param->write.value, param->write.len);
                    buffer[param->write.len] = '\0';
                    // xQueueSend(message_queue, &message_buffer, 10 / portTICK_PERIOD_MS);
                }
                if (param->write.handle == profile_tab[PROFILE_APP_ID].descr_handle[0] && param->write.len == 2) {
                    // CCCD
                    uint16_t descr_value = param->write.value[1] << 8 | param->write.value[0];
                    if (descr_value == 0x0000) {
                        ESP_LOGI(TAG, "Disable notify/indicate");
                        can_notify = false;
                    } else if (descr_value == 0x0001) {
                        ESP_LOGI(TAG, "Enable notify");
                        can_notify = true;
                    } else if (descr_value == 0x0002) {
                        ESP_LOGI(TAG, "Enable indicate");
                        can_notify = true;
                    } else {
                        ESP_LOGE(TAG, "Unknown descriptor value: %04x", descr_value);
                    }
                }
                if (param->write.need_rsp) {
                    esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
                }
            }
            break;
        }
        case ESP_GATTS_EXEC_WRITE_EVT: {
            ESP_LOGI(TAG, "ESP_GATTS_EXEC_WRITE_EVT");
            if (param->exec_write.exec_write_flag) {
                // TODO ???
            }
            break;
        }
        case ESP_GATTS_MTU_EVT: {
            ESP_LOGI(TAG, "ESP_GATTS_MTU_EVT, MTU: %d", param->mtu.mtu);
            break;
        }
        case ESP_GATTS_CONF_EVT: {
            ESP_LOGI(TAG, "ESP_GATTS_CONF_EVT, status: %d, attr_handle: %d", param->conf.status, param->conf.handle);
            if (param->conf.status != ESP_GATT_OK) {
                esp_log_buffer_hex(TAG, param->conf.value, param->conf.len);
            }
            break;
        }
        case ESP_GATTS_UNREG_EVT:
            break;
        case ESP_GATTS_CREATE_EVT: {
            // 服务创建成功, 启动服务
            ESP_LOGI(TAG, "CREATE_SERVICE_EVT, status: %d, service_handle: %d", param->create.status, param->create.service_handle);
            profile_tab[PROFILE_APP_ID].service_handle = param->create.service_handle;
            esp_ble_gatts_start_service(profile_tab[PROFILE_APP_ID].service_handle);
            break;
        }
        case ESP_GATTS_ADD_INCL_SRVC_EVT: 
            break;
        case ESP_GATTS_ADD_CHAR_EVT: {
            // 特征添加成功, 添加描述符
            ESP_LOGI(TAG, "ADD_CHAR_EVT, status: %d, attr_handle: %d, service_handle: %d",
                    param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
            if (param->add_char.char_uuid.uuid.uuid16 == CHAR_RECEIVE_UUID) {
                profile_tab[PROFILE_APP_ID].char_handle[0] = param->add_char.attr_handle;
            } else if (param->add_char.char_uuid.uuid.uuid16 == CHAR_SEND_UUID) {
                profile_tab[PROFILE_APP_ID].char_handle[1] = param->add_char.attr_handle;
                profile_tab[PROFILE_APP_ID].descr_uuid[0].len = ESP_UUID_LEN_16;
                profile_tab[PROFILE_APP_ID].descr_uuid[0].uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
                esp_ble_gatts_add_char_descr(profile_tab[PROFILE_APP_ID].service_handle, &profile_tab[PROFILE_APP_ID].descr_uuid[0],
                                                ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
            }
            break;
        }
        case ESP_GATTS_ADD_CHAR_DESCR_EVT: {
            // 描述符添加成功
            ESP_LOGI(TAG, "ADD_DESCR_EVT, status: %d, attr_handle: %d, service_handle: %d",
                    param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
            profile_tab[PROFILE_APP_ID].descr_handle[0] = param->add_char_descr.attr_handle;
            break;
        }
        case ESP_GATTS_DELETE_EVT:
            break;
        case ESP_GATTS_START_EVT:
            // 服务启动成功, 添加特征
            ESP_LOGI(TAG, "SERVICE_START_EVT, status: %d, service_handle: %d", param->start.status, param->start.service_handle);
            profile_tab[PROFILE_APP_ID].char_uuid[0].len = ESP_UUID_LEN_16;
            profile_tab[PROFILE_APP_ID].char_uuid[0].uuid.uuid16 = CHAR_RECEIVE_UUID;
            esp_ble_gatts_add_char(param->start.service_handle, &profile_tab[PROFILE_APP_ID].char_uuid[0],
                                        ESP_GATT_PERM_WRITE | ESP_GATT_PERM_READ,
                                        ESP_GATT_CHAR_PROP_BIT_WRITE_NR | ESP_GATT_CHAR_PROP_BIT_WRITE,
                                        NULL, NULL);
            profile_tab[PROFILE_APP_ID].char_uuid[1].len = ESP_UUID_LEN_16;
            profile_tab[PROFILE_APP_ID].char_uuid[1].uuid.uuid16 = CHAR_SEND_UUID;
            esp_ble_gatts_add_char(param->start.service_handle, &profile_tab[PROFILE_APP_ID].char_uuid[1],
                                        ESP_GATT_PERM_WRITE | ESP_GATT_PERM_READ,
                                        ESP_GATT_CHAR_PROP_BIT_NOTIFY | ESP_GATT_CHAR_PROP_BIT_INDICATE,
                                        NULL, NULL);
            break;
        case ESP_GATTS_STOP_EVT:
            break;
        case ESP_GATTS_CONNECT_EVT: {
            // 客户端连接
            ESP_LOGI(TAG, "ESP_GATTS_CONNECT_EVT, conn_id: %d, bda: %02x:%02x:%02x:%02x:%02x:%02x",
                    param->connect.conn_id,
                    param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                    param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
            profile_tab[PROFILE_APP_ID].conn_id = param->connect.conn_id;
            memcpy(&remote_bda, &param->connect.remote_bda, sizeof(esp_bd_addr_t));
            can_notify = false;
            is_connected = true;
            // 更新连接参数
            esp_ble_conn_update_params_t conn_params = {
                .latency = 0,
                .max_int = 0x20, // 0x20*1.25ms = 40ms
                .min_int = 0x10, // 0x10*1.25ms = 20ms
                .timeout = 400 // 400*10ms = 4000ms
                };
            memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
            esp_ble_gap_update_conn_params(&conn_params);
            break;
        }
        case ESP_GATTS_DISCONNECT_EVT: {
            // 客户端断开连接
            ESP_LOGI(TAG, "ESP_GATTS_DISCONNECT_EVT");
            is_connected = false;
            esp_ble_gap_config_adv_data_raw(adv_data_raw, sizeof(adv_data_raw));
            break;
        }
        case ESP_GATTS_OPEN_EVT:
            break;
        case ESP_GATTS_CANCEL_OPEN_EVT:
            break;
        case ESP_GATTS_CLOSE_EVT:
            break;
        case ESP_GATTS_LISTEN_EVT:
            break;
        case ESP_GATTS_CONGEST_EVT:
            break;
        default:
            break;
    }
}

void bluetooth_init() {
    ESP_LOGI(TAG, "Initialize Bluetooth");

    profile_tab[PROFILE_APP_ID].gatts_cb = gatts_profile_event_handler;
    profile_tab[PROFILE_APP_ID].gatts_if = ESP_GATT_IF_NONE;

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_ble_gap_set_device_name(CONFIG_ESP_BLUETOOTH_NAME));
    // esp_ble_gap_config_adv_data(&adv_data);
    // esp_ble_gap_config_adv_data(&scan_rsp_data);
    esp_ble_gap_config_adv_data_raw(adv_data_raw, sizeof(adv_data_raw));
    esp_ble_gap_config_scan_rsp_data_raw(adv_data_raw, sizeof(adv_data_raw));
    esp_ble_gap_register_callback(gap_event_handler);
    esp_ble_gatt_set_local_mtu(500);
    esp_ble_gatts_register_callback(gatts_event_handler);
    esp_ble_gatts_app_register(PROFILE_APP_ID);

    // message_queue = xQueueCreate(10, sizeof(uint32_t));
    // xTaskCreate(message_task, "message_task", 2048, NULL, 10, NULL);
    memset(buffer, 0, sizeof(buffer) / sizeof(buffer[0]));
}

bool bluetooth_is_connecting() {
    return is_connected;
}

void bluetooth_send(char *data) {
    if (bluetooth_is_connecting()) {
        uint16_t length = strlen(data);
        esp_ble_gatts_send_indicate(profile_tab[PROFILE_APP_ID].gatts_if, profile_tab[PROFILE_APP_ID].conn_id,
                                        profile_tab[PROFILE_APP_ID].char_handle[1], length, (uint8_t*) data, false);
    }
}

bool bluetooth_receive(char *data) {
    if (bluetooth_is_connecting()) {
        if (buffer[0] != '\0') {
            for (int i = 0; i < 20; ++i) {
                data[i] = buffer[i];
            }
            memset(buffer, 0, sizeof(buffer) / sizeof(buffer[0]));
            return true;
        }
    }
    return false;
}
