#include "transport.h"

Transport_t *transport_init(
  int (*connect)(Transport_t *transport),
  int (*transmit)(Transport_t *transport, Buf_t *buf),
  int (*receive)(Transport_t *transport, Buf_t *buf),
  void *ctx, uint32_t bufsize
) {
  Transport_t *transport = (Transport_t *)malloc(sizeof(Transport_t));
  Buf_t *buf = create_buf(bufsize);

  transport->connect = connect;
  transport->transmit = transmit;
  transport->receive = receive;
  transport->ctx = ctx;
  transport->buf = buf;

  return transport;
}
