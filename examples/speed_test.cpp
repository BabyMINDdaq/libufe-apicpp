#include <iostream>
#include <unistd.h>
#include <thread>

#include "libufe.h"
#include "libufe-tools.h"

using namespace std;

#define BUF_SIZE  2048

#include <string>
#include <chrono>

#define GET_TIME std::chrono::high_resolution_clock::now()
#define GET_DURATION(t0) std::chrono::duration_cast<std::chrono::duration<double>>( \
std::chrono::high_resolution_clock::now()-t0).count()

typedef  std::chrono::high_resolution_clock::time_point  hd_time;

int board_id = 3;

void get_data(libusb_device_handle *dev_handle) {

  uint8_t *data_out = (uint8_t*) malloc(BUF_SIZE);
  int actual_read=0, read_tot=0;

  hd_time t0 = GET_TIME;

  while (ufe_read_buffer(dev_handle, data_out, BUF_SIZE, &actual_read) == 0) {
    if (actual_read == 0)
      break;

    read_tot += actual_read;
  }

  double time = GET_DURATION(t0);
  cout << "\ntime: " << time << "  data: " << read_tot/(1024.*1024)
       << " MB   speed: " << (read_tot/(1024*1024))/time << " MB/s\n\n";
}

int readout(libusb_device_handle *dev_handle) {
  uint16_t data_16 = 0x580;
  data_16 &= 0xfffe;
  int status = ufe_data_readout(dev_handle, board_id, &data_16);

  thread t1(get_data, dev_handle);

  sleep(10);
  data_16 |= 0x1;
  status = ufe_data_readout(dev_handle, board_id, &data_16);

  /* Wait for the readout thread to finish */
  t1.join();

  return status;
}

int main(int argc, char** argv) {

//   ufe_context *ctx(nullptr);
//   ufe_default_context(&ctx);
//   ctx->verbose_ = 3;

  int status = on_board_do(board_id, &readout);
  return (status)? 0 : 1;
}

