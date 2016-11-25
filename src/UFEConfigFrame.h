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

  void LoadConfigFrameFromJsonFile(std::string file);
  void LoadUserConfigFromJsonFile(std::string file, int dev);
  void LoadDirectParamFromJsonFile(std::string file, uint16_t &par);
  void LoadReadoutParamFromJsonFile(std::string file, uint16_t &par);
  void LoadJsonFile(std::string file, Json::Value &json_doc);

  void LoadConfigFrame(const Json::Value &c);
  void LoadUserConfig(const Json::Value &c, unsigned int dev);
  void LoadDirectParam(const Json::Value &c, uint16_t &par);
  void LoadReadoutParam(const Json::Value &c, uint16_t &par);
  int  GetSortedList(std::vector<Variable> &index, unsigned int dev);
  void SetConfigBuffer(unsigned int dev);
  void ResetBuffer() { Buffer_.Reset(); }
  void DumpBuffer();
  void DumpBufferToFile(std::ofstream &file);
  void PrintBuffer();

private:

  void GetBoardConfig(const Json::Value &c);

  void GetAsicConfig(const Json::Value &c);
  void GetAsicChannelsConfig(const Json::Value &c);
  void GetAsicGlobalControlConfig( const Json::Value &conf);
  void GetAsicPowerModesConfig( const Json::Value &conf);

  void GetFpgaConfig(const Json::Value &c);
  void GetFpgaAsicConfig(const Json::Value &c);
  void GetFpgaGlobalControl(const Json::Value &c);

  void GetAsicUserConfig(Json::Value u);
  void GetFpfaUserConfig(Json::Value u);

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

  struct myclass {
    bool operator() (Variable i, Variable j) {
      return (i.MemoryLayout_.Index_ < j.MemoryLayout_.Index_);
    }
  } varSorter_;

};

#endif

