#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined (ESP_PLATFORM)
  #include "esp_system.h"
  #include "esp_timer.h"
#elif (defined __APPLE__)|| (defined __linux__)
  #include <sys/time.h>
#endif

typedef struct Buf_t {
  uint8_t *data;
  uint32_t size;
} Buf_t;

typedef enum result_t {
  RES_OK,
  RES_FAIL
} result_t;

#ifdef __cplusplus
extern "C" {
#endif

void init();
uint64_t get_time();
void loge(const char *tag, const char *fmt, ...);
void logi(const char *tag, const char *fmt, ...);
void logw(const char *tag, const char* fmt, ...);

Buf_t *create_buf(uint32_t size);
void exit_if_null(void *res);
void exit_not_ok(result_t res);

#ifdef __cplusplus
}
#endif

#endif
