#ifndef UFE_CONFIGFRAME_H
#define UFE_CONFIGFRAME_H 1

#include <vector>

#include "UFEConfigLib.h"
#include "UFEConfigBuffer.h"

class UFEConfigFrame {
public:
  UFEConfigFrame() {};
  UFEConfigFrame(Json::Value c);

  virtual ~UFEConfigFrame() {}

  static void loadJsonFile(std::string file, Json::Value &json_doc);

  void loadConfigFrameFromJsonFile(std::string file);
  void loadUserConfigFromJsonFile(std::string file, int dev);
  void loadDirectParamFromJsonFile(std::string file, uint16_t &par);
  void loadReadoutParamFromJsonFile(std::string file, uint16_t &par);

  void loadConfigFrame(const Json::Value &c);
  void loadUserConfig(const Json::Value &c, unsigned int dev);
  void loadDirectParam(const Json::Value &c, uint16_t &par);
  void loadReadoutParam(const Json::Value &c, uint16_t &par);
  int  getSortedList(std::vector<Variable> &index, unsigned int dev);
  void setConfigBuffer(unsigned int dev);
  uint32_t*    getConfigData()     const {return Buffer_.getData();}
  unsigned int getConfigDataSize() const {return Buffer_.getSize();}
  void resetBuffer()              { Buffer_.reset(); }
  void dumpBuffer();
  void dumpBufferToFile(std::ofstream &file);
  void printBuffer();

private:
  void getBoardConfig(const Json::Value &c);

  void getAsicConfig(const Json::Value &c);
  void getAsicChannelsConfig(const Json::Value &c);
  void getAsicGlobalControlConfig( const Json::Value &conf);
  void getAsicPowerModesConfig( const Json::Value &conf);

  void getFpgaConfig(const Json::Value &c);
  void getFpgaAsicConfig(const Json::Value &c);
  void getFpgaGlobalControl(const Json::Value &c);

  void getAsicUserConfig(Json::Value u);
  void getFpfaUserConfig(Json::Value u);

  std::string Name_;
  double Version_;
  int DeviceMemorySize_;
  double MinFpgaVersion_;

  std::vector<HardwareVId> HardwareVIds_;
  std::vector<FirmwareVId> FirmwareVIds_;

  Board Board_;
  ASIC Asic_;
  FPGA Fpga_;
  UFEConfigBuffer Buffer_;

  struct VarSorter {
    bool operator() (Variable i, Variable j) {
      return (i.MemoryLayout_.Index_ < j.MemoryLayout_.Index_);
    }
  } varSorter_;

};

#endif

