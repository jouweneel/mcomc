#include "transport_tcp.h"

static const char *TAG = "tcp";

typedef struct test_t {
  uint8_t a;
  uint32_t b;
  uint16_t c;
} test_t;

int main() {
  init();
  const char *server_ip = "192.168.0.201";

  Transport_t *tcp = transport_tcp();
  tcp_set_server(tcp, server_ip, 2222);
  int res = tcp->connect(tcp);
  if (res < 0) {
    logi(TAG, "not connected\n");
    return -1;
  }

  Buf_t *buf = create_buf(4);
  buf->data[0] = 'h';
  buf->data[1] = 'o';
  buf->data[2] = 'i';
  buf->data[3] = 0;
  buf->size = 4;

  while(1) {
    int len = tcp->receive(tcp, tcp->buf);
    if (len > 0) {
      tcp->transmit(tcp, buf);
    }
  }
  return 0;
}
