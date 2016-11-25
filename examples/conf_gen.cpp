#include <fstream>

#include "libufe-tools.h"

#include "UFEConfigFrame.h"
#include "UFEConfigBuffer.h"
#include "UFEError.h"
#include "UFEDeff.h"

using namespace std;

void print_usage(char *argv) {
  fprintf(stderr, "\nUsage: %s [OPTIONS] \n\n", argv);
  fprintf(stderr, "    -i / --device-id   <int dec/hex>  ( Configuration for device Id )                          [ optional OR a/f/d ]\n");
  fprintf(stderr, "    -a / --asics                      ( Configuration fot the 3 asics )                        [ optional OR i/f/d ]\n");
  fprintf(stderr, "    -f / --fpga                       ( Configuration for the fpga )                           [ optional OR i/a/d ]\n");
  fprintf(stderr, "    -d / --all-devices                ( Configuration for all devices )                        [ optional OR i/a/f ]\n");
  fprintf(stderr, "    -c / --frame-descr   <string>     ( Json file containing the configuration frame descr. )  [ optional ]\n");
  fprintf(stderr, "    -u / --user-config   <string>     ( Json file containing the user configuration )          [ optional ]\n");
  fprintf(stderr, "    -o / --output-file   <string>     ( Name of the output file containing the configuration ) [ optional ]\n");
//   fprintf(stderr, "    -s / --stdout                        ( Config bit array to stdout )                           [ optional ]\n\n");
}

int main (int argc, char **argv) {

  int device_id_arg   = get_arg_val('i', "device-id"   , argc, argv);
  int asics_arg           = get_arg('a', "asics"       , argc, argv);
  int fpga_arg            = get_arg('f', "fpga"        , argc, argv);
  int all_devices_arg     = get_arg('d', "all-devices" , argc, argv);
  int frame_arg       = get_arg_val('c', "frame-descr" , argc, argv);
  int user_arg        = get_arg_val('u', "user-config" , argc, argv);
  int out_arg         = get_arg_val('o', "output-file" , argc, argv);
//   int pipe_arg        = get_arg('s',     "stdout"        , argc, argv);

  if (device_id_arg == 0 && all_devices_arg == 0 && asics_arg == 0 && fpga_arg == 0) {
    print_usage(argv[0]);
    return 1;
  }

  string config_descr_file(UFEAPI_DIR);
  config_descr_file += "/conf/config-desc.json";
  string config_user_file(UFEAPI_DIR);
  config_user_file += "/conf/PDdiscri-A2-CH5.json";

  if (frame_arg != 0) {
    config_descr_file = argv[frame_arg];
  }

  if (user_arg != 0) {
    config_user_file = argv[user_arg];
  }

  ofstream file;
  if (out_arg !=0 )
    file.open(argv[out_arg]);

  try {
    UFEConfigFrame c;
    c.LoadConfigFrameFromJsonFile(config_descr_file);

    if (device_id_arg != 0 && asics_arg == 0 && fpga_arg == 0 && all_devices_arg == 0) {
      int device_id = arg_as_int(argv[device_id_arg]);
      c.LoadUserConfigFromJsonFile(config_user_file, device_id);
      c.SetConfigBuffer(device_id);
      if (out_arg != 0) c.DumpBufferToFile(file);
      else c.DumpBuffer();
    } else if (device_id_arg == 0 && asics_arg == 0 && fpga_arg != 0 && all_devices_arg == 0) {
      int device_id = 3;
      c.LoadUserConfigFromJsonFile(config_user_file, device_id);
      c.SetConfigBuffer(device_id);
      if (out_arg != 0) c.DumpBufferToFile(file);
      else c.DumpBuffer();;
    } else if (device_id_arg == 0 && asics_arg != 0 && fpga_arg == 0 && all_devices_arg == 0) {
      for (int xDev=0; xDev<3; ++xDev) {
        c.LoadUserConfigFromJsonFile(config_user_file, xDev);
        c.SetConfigBuffer(xDev);
        if (out_arg != 0) c.DumpBufferToFile(file);
        else c.DumpBuffer();
      }
    } else if ( all_devices_arg != 0 ||
               (asics_arg != 0 && fpga_arg != 0 && all_devices_arg) ) {
      for (int xDev=0; xDev<4; ++xDev) {
        c.LoadUserConfigFromJsonFile(config_user_file, xDev);
        c.SetConfigBuffer(xDev);
        if (out_arg != 0) c.DumpBufferToFile(file);
        else c.DumpBuffer();
      }
    } else {
      print_usage(argv[0]);
      return 1;
    }
  } catch (UFEError &e) {
    cerr << e.GetDescription() << endl;
    cerr << e.GetLocation() << endl;
    return 1;
  }

  if (out_arg !=0 )
    file.close();

  return 0;
}