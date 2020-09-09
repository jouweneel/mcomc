#include "transport_udp.h"
#include "esp_log.h"

static const char *TAG = "transport_udp";

static int udp_transmit(Transport_t *transport, Buf_t *buf) {
  TransportUdpCtx_t *udp = (TransportUdpCtx_t *)(transport->ctx);
  return sendto(udp->sock, buf->data, buf->size, 0, (struct sockaddr *)udp->target, sizeof(struct sockaddr));
}

static int udp_receive(Transport_t *transport, Buf_t *buf) {
  TransportUdpCtx_t *udp = (TransportUdpCtx_t *)(transport->ctx);

  socklen_t len = sizeof(struct sockaddr);
  int res = recvfrom(udp->sock, buf->data, buf->size, 0, (struct sockaddr *)udp->host, &len);
  if (res <= 0) {
    loge(TAG, "receive: %i\n", res);
    buf->size = 0;
  }
  buf->size = res;

  return res;
}

Transport_t *transport_udp(void *config) {
  TransportUdpCfg_t *cfg = (TransportUdpCfg_t *)config;

  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    loge(TAG, "opening socket\n");
    return NULL;
  }

  int bcastEnable = 1;
  int res = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bcastEnable, sizeof(bcastEnable));
  
  if (res < 0) {
    return NULL;
  }

  socklen_t addr_size = sizeof(struct sockaddr_in);
  struct sockaddr_in *host = (struct sockaddr_in *)malloc(addr_size);
  struct sockaddr_in *target = (struct sockaddr_in *)malloc(addr_size);

  memset((void *)host, 0, addr_size);
  memset((void *)target, 0, addr_size);

  host->sin_addr.s_addr = htonl(INADDR_ANY);
  host->sin_family = AF_INET;
  host->sin_port = htons(cfg->port);

  target->sin_family = AF_INET;

  TransportUdpCtx_t *ctx = (TransportUdpCtx_t *)malloc(sizeof(TransportUdpCtx_t));
  ctx->sock = sock;
  ctx->host = host;
  ctx->target = target;

  res |= bind(sock, (struct sockaddr *)host, addr_size);

  return transport_init(udp_transmit, udp_receive, (void *)ctx, cfg->bufsize);
}

void transport_udp_target(Transport_t *transport, const char *ip, in_port_t port) {
  TransportUdpCtx_t *udp = (TransportUdpCtx_t *)transport->ctx;
  udp->target->sin_addr.s_addr = inet_addr(ip);
  udp->target->sin_port = htons(port);
}
