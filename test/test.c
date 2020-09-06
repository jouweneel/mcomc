#include "mcom.h"
#include "mcom_state.h"
#include "transport_udp.h"

static const char *TAG = "test";

// static const char *data = "ping!";
static McomMsg_t msg[] = {{
  .cmd = 1,
  .type = DATA_TYPE_STRING,
  .length = 5,
  .data = (uint8_t *)"ping!"
}, {
  .cmd = 2,
  .type = DATA_TYPE_I16,
  .length = 1,
  .data = NULL
}};
int16_t int16 = -4000;

TransportUdpCfg_t udp_cfg = { .port = 3333, .bufsize = 1500 };

void cmd0_handler(McomMsg_t *msg) {
  uint8_t *data = msg->data;
  logi(TAG, "handler 0: [%u, %u, %u]\n", data[0], data[1], data[2]);
  data[0] = 10;
}
void cmd1_handler(McomMsg_t *msg) {
  int16_t *data = (int16_t *)msg->data;
  logi(TAG, "handler 1: [%i, %i]\n", data[0], data[1]);
}

int main() {
  init();
  Mcom_t *mcom_udp = Mcom(&protocol_bs, transport_udp, (void *)(&udp_cfg));
  transport_udp_target(mcom_udp->transport, "127.0.0.1", 2222);

  Mcom_add_handler("cmd0", DATA_TYPE_HSV, 1, MCOM_RW, cmd0_handler);
  Mcom_add_handler("cmd1", DATA_TYPE_I16_A, 2, MCOM_RW, cmd1_handler);

  McomMsgs_t transmitData = { .len = 2, .msgs = msg };
  McomMsgs_t *receiveData = NULL;
  msg[1].data = (uint8_t *)(&int16);

  Mcom_transmit(mcom_udp, &transmitData);

  while(1) {
    Mcom_handle(mcom_udp, Mcom_receive(mcom_udp));
  }
}
