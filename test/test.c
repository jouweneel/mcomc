#include "mcom.h"
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

int main() {
  init();
  Mcom_t *mcom_udp = MCom(&protocol_bs, transport_udp, (void *)(&udp_cfg));
  transport_udp_target(mcom_udp->transport, "127.0.0.1", 2222);

  McomMsgs_t transmitData = { .len = 2, .msgs = msg };
  McomMsgs_t *receiveData = NULL;
  msg[1].data = (uint8_t *)(&int16);

  while(1) {
    MCom_transmit(mcom_udp, &transmitData);
    receiveData = MCom_receive(mcom_udp);

    if (receiveData != NULL) {
      for (uint8_t i = 0; i < receiveData->len; i++) {
        McomMsg_t msg = receiveData->msgs[i];
        logi(TAG, "cmd: %u, type: %x, length: %u, data: [ ", msg.cmd, msg.type, msg.length);
        for (uint32_t j = 0; j < msg.length; j++) {
          printf("%x ", msg.data[j]);
        }
        printf("]\n");
      }
    }
  }
}
