#include "mcom_state.h"
#include "mcom_util.h"

#include "esp_log.h"

static const char *TAG = "[mcom_state]";

static uint8_t state_cmd = 0;
static State_t *states = NULL;
static State_t *current_state = NULL;

State_t *get_state(uint8_t cmd) {
  State_t *state = states;

  while (state->msg->cmd != cmd) {
    if (state->next == NULL) {
      return NULL;
    }
    state = state->next;
  }

  return state;
}

void Mcom_handle(Mcom_t *mcom, McomMsg_t *msg) {
  State_t *state = get_state(msg->cmd);
  if (state != NULL) {
    state->handler(msg);
    memcpy(state->msg->data, msg->data, state->data_bytes);
  } else {
    loge(TAG, "No state handler for cmd %u\n", msg->cmd);
  }

  Mcom_transmit(mcom, msg);
  free(msg->data);
}

State_t *Mcom_add_handler(
  char *label,
  DataType_t type,
  uint16_t length,
  Direction_t direction,
  void (*handler)(McomMsg_t *)
) {
  uint8_t unit_size = (type & 0xf0) >> 4;
  uint8_t is_array = (type & 0x08) == 0x08;
  uint32_t data_bytes = is_array ? length * unit_size : unit_size;

  uint8_t *data_buf = (uint8_t *)malloc(data_bytes);

  McomMsg_t *msg = (McomMsg_t *)malloc(sizeof(McomMsg_t));
  msg->cmd = state_cmd;
  msg->type = type;
  msg->length = length;
  msg->data = data_buf;

  State_t *state = (State_t *)malloc(sizeof(State_t));
  state->msg = msg;
  state->data_bytes = data_bytes;
  state->label = label;
  state->direction = direction;
  state->handler = handler;
  state->next = NULL;

  if (states == NULL) {
    states = state;
  } else {
    current_state->next = state;
  }

  current_state = state;
  state_cmd++;

  return state;
}

void Mcom_update_state(Mcom_t *mcom, State_t *state, uint8_t *value) {
  memcpy(state->msg->data, value, state->data_bytes);
  Mcom_transmit(mcom, state->msg);
}
