#include <iostream>
#include <unistd.h>
#include <thread>

#include "libufe.h"
#include "libufe-tools.h"

using namespace std;

#define BUF_SIZE  (1024*64)

#include <string>
#include <chrono>

#define GET_TIME std::chrono::high_resolution_clock::now()
#define GET_DURATION(t0) std::chrono::duration_cast<std::chrono::duration<double>>( \
std::chrono::high_resolution_clock::now()-t0).count()

typedef  std::chrono::high_resolution_clock::time_point  hd_time;
int board_id, time_s;

void get_data(libusb_device_handle *dev_handle) {

  uint8_t *data_out = (uint8_t*) malloc(BUF_SIZE);
  uint64_t read_tot=0;
  int actual_read=0;

  hd_time t0 = GET_TIME;

  while (ufe_read_buffer(dev_handle, data_out, &actual_read) == 0) {
    if (actual_read == 0)
      break;

    read_tot += actual_read;
  }

  double time = GET_DURATION(t0);
  cerr << "\ntime: " << time << "  data: " << read_tot/(1024.*1024)
       << " MB   speed: " << (read_tot/(1024*1024))/time << " MB/s\n\n";
}

int readout(libusb_device_handle *dev_handle) {
  uint16_t data_16 = 0x580;
  data_16 &= 0xfffe;
  int status = ufe_data_readout(dev_handle, board_id, &data_16);

  thread t1(get_data, dev_handle);

  sleep(time_s);
//   for (int i=0;i<time_s;++i) {
//     sleep(1);
//     uint16_t st(0);
//     ufe_read_status(dev_handle, board_id, &st);
//     cerr << "fif err: " << (!!(status & RS_L0F_ERR))
//          << " / "       << (!!(status & RS_L1F_ERR))
//          << " / "       << (!!(status & RS_L2F_ERR)) << endl;
//   }

  data_16 |= 0x1;
  status = ufe_data_readout(dev_handle, board_id, &data_16);

  /* Wait for the readout thread to finish */
  t1.join();

  return status;
}

void print_usage(char *argv) {
  fprintf(stderr, "\nUsage: %s [OPTION] ARG \n\n", argv);
  fprintf(stderr, "    -b / --board-id     <int dec/hex>   ( Board Id )                  [ required ]\n");
  fprintf(stderr, "    -t / --time         <int dec/hex>   ( Duration in seconds )       [ optional / Default 10 s ]\n");
}

int main(int argc, char** argv) {

  int board_id_arg = get_arg_val('b', "board-id"    , argc, argv);
  int time_arg     = get_arg_val('t', "time"        , argc, argv);

  if (board_id_arg == 0) {
    print_usage(argv[0]);
    return 1;
  }

  board_id = arg_as_int(argv[board_id_arg]);

  time_s = 10;
  if (time_arg != 0)
    time_s = arg_as_int(argv[time_arg]);

  ufe_context *ctx(nullptr);
  ufe_default_context(&ctx);
  ctx->verbose_ = 1;
  ctx->readout_timeout_ = 50;
  int status = ufe_on_board_do(board_id, &readout);
  return (status)? 0 : 1;
}

