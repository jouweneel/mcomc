#include "config.h"
#include "transport_mqtt.h"

static const char *TAG = "transport_mqtt";

#if defined (ESP_PLATFORM)

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
  esp_mqtt_client_handle_t client = event->client;
  TransportMqttCtx_t *ctx = (TransportMqttCtx_t *)(event->user_context);

  switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
      logi(TAG, "MQTT_EVENT_CONNECTED");

      esp_mqtt_client_subscribe(client, "/request/" ESP_NAME "/#", 0);
      break;
    case MQTT_EVENT_DATA:
      logi(TAG, "MQTT_EVENT_DATA");

      memcpy(ctx->buf->data, event->data, event->data_len);
      ctx->buf->size = event->data_len;
      xTaskNotifyGive(ctx->handle);
      break;
    default:
      return ESP_FAIL;
  }
  return ESP_OK;
}

static int mqtt_transmit(Transport_t *transport, Buf_t *buf) {
  TransportMqttCtx_t *ctx = (TransportMqttCtx_t *)(transport->ctx);
  esp_mqtt_client_handle_t client = (esp_mqtt_client_handle_t)(ctx->client);
  
  int res = esp_mqtt_client_publish(client, "/respond/" ESP_NAME, (char *)(buf->data), buf->size, 0, 1);
  if (res >= 0) {
    return (int)(buf->size);
  } else {
    return -1;
  }
}

static int mqtt_receive(Transport_t *transport, Buf_t *buf) {
  ulTaskNotifyTake(1, portMAX_DELAY);
  logi(TAG, "received mqtt data");
  return (int)(buf->size);
}

Transport_t *transport_mqtt(void *config) {
  TransportMqttCfg_t *cfg = (TransportMqttCfg_t *)config;

  TransportMqttCtx_t *ctx = (TransportMqttCtx_t *)malloc(sizeof(TransportMqttCtx_t));
  ctx->handle = xTaskGetCurrentTaskHandle();

  Transport_t *transport = transport_init(mqtt_transmit, mqtt_receive, (void *)ctx, 1500);
  ctx->buf = transport->buf;

  esp_mqtt_client_config_t mqtt_cfg = {
    .uri = cfg->url,
    .event_handle = mqtt_event_handler,
    .user_context = (void *)ctx
  };
  esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
  ctx->client = client;

  esp_mqtt_client_start(client);

  return transport;
}

#endif
