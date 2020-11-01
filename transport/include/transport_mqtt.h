#ifndef _TRANSPORT_MQTT_H_
#define _TRANSPORT_MQTT_H_

#if defined (ESP_PLATFORM)
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "mqtt_client.h"
#endif

#include "transport.h"

typedef struct TransportMqttCfg_t {
  char *url;
} TransportMqttCfg_t;

#if defined (ESP_PLATFORM)
  typedef struct TransportMqttCtx_t {
    Buf_t *buf;
    esp_mqtt_client_handle_t client;
    TaskHandle_t handle;
  } TransportMqttCtx_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

Transport_t *transport_mqtt(void *cfg);

#ifdef __cplusplus
}
#endif

#endif
