#ifndef __WIFI_H__
#define __WIFI_H__

#include "esp_wifi_types.h"
#include "esp_wifi.h"

void wifi_init();
void wifi_init_ap();
void wifi_init_sta(const char* ssid, const char* password);

#endif
