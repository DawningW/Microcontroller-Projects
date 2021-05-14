idf_component_register(SRCS "http_server.c" "bluetooth.c" "wifi.c" "main.c"
                    INCLUDE_DIRS "")

spiffs_create_partition_image(storage ../root FLASH_IN_PROJECT)