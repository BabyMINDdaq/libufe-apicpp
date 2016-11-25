#include <fstream>

#include "libufe-tools.h"

#include "UFEConfigFrame.h"
#include "UFEConfigBuffer.h"
#include "UFEError.h"
#include "UFEDeff.h"

using namespace std;

void print_usage(char *argv) {
  fprintf(stderr, "\nUsage: %s [OPTIONS] \n\n", argv);
  fprintf(stderr, "    -f / --frame-descr     <string>    ( Json file containing the configuration frame descr. )  [ optional ]\n");
  fprintf(stderr, "    -u / --user-config     <string>    ( Json file containing the user configuration )          [ optional ]\n");
  fprintf(stderr, "    -o / --output-file     <string>    ( Name of the output file containing the configuration ) [ optional ]\n");
  fprintf(stderr, "    -d / --direct-params               ( Generate direct parameters )                           [ optional OR r ]\n");
  fprintf(stderr, "    -r / --readout-params              ( Generate readout parameters )                          [ optional OR d ]\n");
//   fprintf(stderr, "    -s / --stdout                        ( Config bit array to stdout )                           [ optional ]\n\n");
}

int main (int argc, char **argv) {

  int frame_arg = get_arg_val('f', "frame"         , argc, argv);
  int user_arg  = get_arg_val('u', "user-config"   , argc, argv);
  int out_arg   = get_arg_val('o', "output-file"   , argc, argv);
  int direct_arg    = get_arg('d', "direct-params"   , argc, argv);
  int readout_arg   = get_arg('r', "readout-params"  , argc, argv);

  if (direct_arg == 0 && readout_arg == 0) {
    print_usage(argv[0]);
    return 1;
  } else if (direct_arg != 0 && readout_arg != 0) {
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

  try {

    UFEConfigFrame c;
    uint16_t par;
    c.LoadConfigFrameFromJsonFile(config_descr_file);
    if (direct_arg)
      c.LoadDirectParamFromJsonFile(config_user_file, par);
    else
      c.LoadReadoutParamFromJsonFile(config_user_file, par);

    if (out_arg != 0) {
      ofstream out_file(argv[out_arg]);
      out_file << "0x" << hex << par << endl;
      out_file.close();
    } else
      cout << "0x" << hex << par << dec << endl;

  } catch (UFEError &e) {
    cerr << e.GetDescription() << endl;
    cerr << e.GetLocation() << endl;
    return 1;
  }

  return 0;
}