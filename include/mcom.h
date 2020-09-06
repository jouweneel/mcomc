#ifndef _MCOM_H_
#define _MCOM_H_

#include "transport.h"
#include "mcom_types.h"
#include "mcom_util.h"

typedef struct Mcom_t {
  Transport_t *transport;
  Protocol_t *protocol;
} Mcom_t;

extern Protocol_t protocol_bs;

#ifdef __cplusplus
extern "C" {
#endif

Mcom_t *Mcom(Protocol_t *protocol, TransportFn_t transport_fn, void *cfg);
uint32_t Mcom_transmit(Mcom_t *mcom, McomMsgs_t *msgs);
McomMsgs_t *Mcom_receive(Mcom_t *mcom);

#ifdef __cplusplus
}
#endif

#endif
