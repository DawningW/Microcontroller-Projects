#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
// Class bt
// #include "esp_gap_bt_api.h"
// #include "esp_bt_device.h"
// #include "esp_spp_api.h"
// BLE
#include "esp_gap_ble_api.h"
#include "esp_gatt_common_api.h"
#include "esp_gatts_api.h"

struct gatts_profile_instance {
    uint16_t app_id;
    esp_gatts_cb_t gatts_cb;
    esp_gatt_if_t gatts_if;
    esp_gatt_srvc_id_t service_id;
    uint16_t service_handle;
    esp_bt_uuid_t char_uuid[2];
    uint16_t char_handle[2];
    esp_bt_uuid_t descr_uuid[1];
    uint16_t descr_handle[1];
    uint16_t conn_id;
};

void bluetooth_init();
bool bluetooth_is_connecting();
void bluetooth_send(char *data);
bool bluetooth_receive(char *data);

#endif
