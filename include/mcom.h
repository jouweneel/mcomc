#ifndef _MCOM_H_
#define _MCOM_H_

#include "mcom_types.h"
#include "mcom_util.h"
#include "protocol.h"
#include "transport.h"

typedef struct Mcom_t {
  Transport_t *transport;
  Protocol_t *protocol;
} Mcom_t;

#ifdef __cplusplus
extern "C" {
#endif

Mcom_t *MCom(Protocol_t *protocol, TransportFn_t transport_fn, void *cfg);
uint32_t MCom_transmit(Mcom_t *mcom, McomMsgs_t *msgs);
McomMsgs_t *MCom_receive(Mcom_t *mcom);

#ifdef __cplusplus
}
#endif

#endif
