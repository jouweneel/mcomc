#ifndef _TRANSPORT_UDP_H_
#define _TRANSPORT_UDP_H_

#if defined (ESP_PLATFORM)
  #include "lwip/inet.h"
  #include "lwip/sockets.h"
#elif (defined __APPLE__)|| (defined __linux__)
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
#endif

#include "transport.h"

typedef struct TransportUdpCtx_t {
  int sock;
  struct sockaddr_in *host;
  struct sockaddr_in *target;
} TransportUdpCtx_t;

typedef struct TransportUdpCfg_t {
  in_port_t port;
  uint32_t bufsize;
} TransportUdpCfg_t;

#ifdef __cplusplus
extern "C" {
#endif

Transport_t *transport_udp(void *cfg);
void transport_udp_target(Transport_t *transport, const char *ip, in_port_t port);

#ifdef __cplusplus
}
#endif

#endif
