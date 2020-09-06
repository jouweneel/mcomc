#include <string.h>

#include "protocol.h"
#include "mcom_util.h"

static const char *TAG = "bs";

McomMsgs_t *bs_decode(Buf_t *buf) {
  uint8_t *data = buf->data;

  uint32_t ptr = 0;
  McomMsg_t *msgs = NULL;
  uint8_t nr_msgs = 0;

  while (ptr < buf->size) {
    nr_msgs++;
    msgs = (McomMsg_t *)realloc(msgs, sizeof(McomMsg_t) * nr_msgs);
    McomMsg_t *msg = &(msgs[nr_msgs - 1]);

    msg->cmd = data[ptr];
    msg->type = data[ptr + 1];

    uint8_t unit_size = (msg->type & 0xf0) >> 4;
    uint8_t is_array = (msg->type & 0x08) == 0x08;

    uint16_t length = is_array ? ((uint16_t *)data)[1] : 1;
    uint8_t header_size = is_array ? 4 : 2;
    uint32_t data_size = unit_size * length;

    msg->length = length;
    msg->data = (uint8_t *)malloc(data_size);
    memcpy(msg->data, &(data[ptr + header_size]), data_size);

    ptr += (header_size + data_size);
  }

  McomMsgs_t *output = (McomMsgs_t *)malloc(sizeof(McomMsgs_t *));
  output->len = nr_msgs;
  output->msgs = msgs;

  return output;
}

Buf_t *bs_encode(McomMsgs_t *msgs) {
  uint32_t ptr = 0;
  uint8_t *outbuf = NULL;

  for (uint8_t i = 0; i < msgs->len; i++) {
    McomMsg_t *msg = &(msgs->msgs[i]);

    uint8_t unit_size = (msg->type & 0xf0) >> 4;
    uint8_t is_array = (msg->type & 0x08) == 0x08;

    uint8_t header_size = is_array ? 4 : 2;
    uint16_t data_size = is_array ? (unit_size * msg->length) : unit_size;

    outbuf = (uint8_t *)realloc(outbuf, header_size + data_size);
    outbuf[ptr] = msg->cmd;
    outbuf[ptr + 1] = msg->type;

    if (is_array) {
      memcpy(&(outbuf[ptr + 2]), &(msg->length), sizeof(uint16_t));
    }
    memcpy(&(outbuf[ptr + header_size]), msg->data, data_size);

    ptr += (header_size + data_size);
  }

  Buf_t *buf = (Buf_t *)malloc(sizeof(Buf_t));
  buf->data = outbuf;
  buf->size = ptr;

  return buf;
}

Protocol_t protocol_bs = {
  .decode = bs_decode,
  .encode = bs_encode
};
