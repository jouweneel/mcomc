#include "esp_log.h"
#include "mcom.h"

Mcom_t *Mcom(Protocol_t *protocol, TransportFn_t transport_fn, void *cfg) {
  Mcom_t *mcom = (Mcom_t *)malloc(sizeof(Mcom_t));
  mcom->transport = transport_fn(cfg);
  mcom->protocol = protocol;

  return mcom;
}

uint32_t Mcom_transmit(Mcom_t *mcom, McomMsg_t *msg) {
  Buf_t *buf = mcom->protocol->encode(msg);
  uint32_t res = mcom->transport->transmit(mcom->transport, buf);
  free(buf->data);
  free(buf);
  return res;
}

McomMsg_t *Mcom_receive(Mcom_t *mcom) {
  int res = mcom->transport->receive(mcom->transport, mcom->transport->buf);
  if (res >= 0) {
    return mcom->protocol->decode(mcom->transport->buf);
  } else {
    return NULL;
  }
}
