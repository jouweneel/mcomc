#ifndef _TRANSPORT_TCP_H_
#define _TRANSPORT_TCP_H_

#if defined (ESP_PLATFORM)
  #include "lwip/inet.h"
  #include "lwip/sockets.h"
#elif (defined __APPLE__)|| (defined __linux__)
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
#endif

#include "transport.h"

typedef struct TransportTcpCtx_t {
  struct sockaddr_in *server;
  int sock;
} TransportTcpCtx_t;

Transport_t *transport_tcp();
void tcp_set_server(Transport_t *transport, const char *ip, in_port_t port);

#endif
