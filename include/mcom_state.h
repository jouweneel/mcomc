#ifndef _MCOM_STATE_H_
#define _MCOM_STATE_H_

#include "mcom.h"

typedef enum Direction_t { MCOM_RO, MCOM_RW } Direction_t;

typedef struct State_t {
  McomMsg_t *data;
  uint32_t data_bytes;

  char *label;
  Direction_t direction;
  void (*handler)(McomMsg_t *);

  struct State_t *next;
} State_t;

#ifdef __cplusplus
extern "C" {
#endif

State_t *Mcom_add_handler(
  char *label,
  DataType_t type,
  uint16_t length,
  Direction_t direction,
  void (*handler)(McomMsg_t *)
);

void Mcom_handle(Mcom_t *mcom, McomMsgs_t *msgs);
void Mcom_update_state(Mcom_t *mcom, State_t *state, uint8_t *value);

#ifdef __cplusplus
}
#endif

#endif
