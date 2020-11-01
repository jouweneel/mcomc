#include <string.h>

#include "mcom.h"

static const char *TAG = "bs";

McomMsg_t *bs_decode(Buf_t *buf) {
  uint8_t *data = buf->data;

  McomMsg_t *msg = (McomMsg_t *)malloc(sizeof(McomMsg_t));

  msg->cmd = data[0];
  msg->type = data[1];

  uint8_t is_array = (msg->type & 0x08) == 0x08;
  uint8_t unit_size = (msg->type & 0xf0) >> 4;

  uint16_t length = is_array ? ((uint16_t *)data)[2] : 1;
  uint8_t header_size = is_array ? 4 : 2;
  uint32_t data_size = unit_size * length;

  msg->length = length;
  msg->data = (uint8_t *)malloc(data_size);

  memcpy(msg->data, &(data[header_size]), data_size);

  return msg;
}

Buf_t *bs_encode(McomMsg_t *msg) {
  uint32_t ptr = 0;

  uint8_t unit_size = (msg->type & 0xf0) >> 4;
  uint8_t is_array = (msg->type & 0x08) == 0x08;

  uint8_t header_size = is_array ? 4 : 2;
  uint16_t data_size = is_array ? (unit_size * msg->length) : unit_size;

  uint8_t *outbuf = (uint8_t *)malloc(header_size + data_size);
  outbuf[ptr] = msg->cmd;
  outbuf[ptr + 1] = msg->type;

  if (is_array) {
    memcpy(&(outbuf[ptr + 2]), &(msg->length), sizeof(uint16_t));
  }
  memcpy(&(outbuf[ptr + header_size]), msg->data, data_size);
  ptr += (header_size + data_size);

  Buf_t *buf = (Buf_t *)malloc(sizeof(Buf_t));
  buf->data = outbuf;
  buf->size = ptr;

  return buf;
}

Protocol_t protocol_bs = {
  .decode = bs_decode,
  .encode = bs_encode
};
