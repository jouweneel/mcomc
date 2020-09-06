#include "mcom_util.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

uint64_t start = 0;

uint64_t get_time() {
#if defined (ESP_PLATFORM)
  return esp_timer_get_time();
#elif (defined __APPLE__)|| (defined __linux__)
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec - start;
#endif
}

void init() {
  start = get_time();
}

void loge(const char *tag, const char* fmt, ...) {
  printf(RED "%10lu [%s] ERROR: ", get_time(), tag);
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
}

void logi(const char *tag, const char* fmt, ...) {
  printf(GRN "%10lu [%s]: " RESET, get_time(), tag);
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
}

void logw(const char *tag, const char* fmt, ...) {
  printf(YEL "%10lu [%s] WARNING: " RESET, get_time(), tag);
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
}

Buf_t *create_buf(uint32_t size) {
  uint8_t *data = (uint8_t *)malloc(size * sizeof(uint8_t));
  Buf_t *buf = (Buf_t *)malloc(sizeof(Buf_t));

  buf->data = data;
  buf->size = size;
  return buf;
}

void exit_if_null(void *res)  {
  if (res == NULL) {
    loge("[exit]", "Null value");
#if defined (ESP_PLATFORM)
  esp_restart();
#else
    exit(EXIT_FAILURE);
#endif
  }
}

void exit_not_ok(result_t res) {
  if (res != RES_OK) {
    loge("[exit]", "Non-OK result");
#if defined (ESP_PLATFORM)
  esp_restart();
#else
    exit(EXIT_FAILURE);
#endif
  }
}
