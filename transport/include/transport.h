#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <stdlib.h>
#include <string.h>
#include "mcom_util.h"

#if defined (ESP_PLATFORM)
  #include "lwip/inet.h"
  #include "lwip/sockets.h"
#elif (defined __APPLE__)|| (defined __linux__)
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
#endif

typedef struct Transport_t {
  int (*connect)(Transport_t *transport);
  int (*transmit)(Transport_t *transport, Buf_t *buf);
  int (*receive)(Transport_t *transport, Buf_t *buf);

  void *ctx;
  Buf_t *buf;
} Transport_t;

typedef Transport_t *(* TransportFn_t)(void *cfg);

#ifdef __cplusplus
extern "C" {
#endif

Transport_t *transport_init(
  int (*connect)(Transport_t *transport),
  int (*transmit)(Transport_t *transport, Buf_t *buf),
  int (*receive)(Transport_t *transport, Buf_t *buf),
  void *ctx, uint32_t bufsize
);

#ifdef __cplusplus
}
#endif

#endif
