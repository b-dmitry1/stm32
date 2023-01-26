#ifndef ESP8266CUSTOM_H
#define ESP8266CUSTOM_H

#define ESP_UART	2

int http_get(const char *url, void *buffer, int size);
void wifi_set_name_password(const char *name, const char *password);

#endif
