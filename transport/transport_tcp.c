#include "transport_tcp.h"

int tcp_transmit(Transport_t *transport, Buf_t *buf) {
  TransportTcpCtx_t *ctx = (TransportTcpCtx_t *)(transport->ctx);
  return send(ctx->sock, buf->data, buf->size, 0);
}

int tcp_receive(Transport_t *transport, Buf_t *buf) {
  TransportTcpCtx_t *ctx = (TransportTcpCtx_t *)(transport->ctx);
  return recv(ctx->sock, buf->data, buf->size, 0);
}

void tcp_set_server(Transport_t *transport, const char *ip, in_port_t port) {
  TransportTcpCtx_t *ctx = (TransportTcpCtx_t *)(transport->ctx);
  ctx->server->sin_addr.s_addr = inet_addr(ip);
  ctx->server->sin_port = htons(port);
  return;
}

void tcp_set_server_addr(Transport_t *transport, struct sockaddr_in *addr) {
  TransportTcpCtx_t *ctx = (TransportTcpCtx_t *)(transport->ctx);
  ctx->server->sin_addr.s_addr = addr->sin_addr.s_addr;
  ctx->server->sin_port = addr->sin_port;
  return;
}

Transport_t *transport_tcp() {
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (sock < 0) {
    return NULL;
  }

  socklen_t addr_size = sizeof(struct sockaddr_in);
  struct sockaddr_in *server = (struct sockaddr_in *)malloc(addr_size);
  memset((void *)server, 0, addr_size);
  server->sin_family = AF_INET;

  TransportTcpCtx_t *ctx = (TransportTcpCtx_t *)malloc(sizeof(TransportTcpCtx_t));
  ctx->server = server;
  ctx->sock = sock;

  connect(ctx->sock, (struct sockaddr *)(ctx->server), addr_size);

  return transport_init(tcp_transmit, tcp_receive, (void *)ctx, 1500);
}
