#ifndef UFE_CONFIG_LIB_H
#define UFE_CONFIG_LIB_H 1

#include <string>
#include <iostream>
#include <vector>
#include <jsoncpp/json/json.h>

#define THROW_ERROR(location, json_doc, key) { \
  std::string message; \
  if (json_doc.isMember("Name")) { \
    message += "When reading "; \
    message += json_doc["Name"].asString();} \
  message += " Compulsory member >>"; \
  message += #key; \
  message += "<< is missing in the json configuration!"; \
  throw UFEError(message, #location, UFEError::FATAL);}

#define SET_MEMBER_BOOL(obj, json_doc, key, compulsory) \
{ if (json_doc.isMember(#key)) obj->key##_ = json_doc[#key].asBool(); \
  else if (compulsory) THROW_ERROR(SET_MEMBER_DOUBLE_ASSTRING, json_doc, key)}

#define SET_MEMBER_INT(obj, json_doc, key, compulsory) \
{ if (json_doc.isMember(#key)) obj->key##_ = json_doc[#key].asInt(); \
  else if (compulsory) THROW_ERROR(SET_MEMBER_INT, json_doc, key)}

#define SET_MEMBER_DOUBLE(obj, json_doc, key, compulsory) \
{ if (json_doc.isMember(#key)) obj->key##_ = json_doc[#key].asDouble(); \
  else if (compulsory) THROW_ERROR(SET_MEMBER_DOUBLE, json_doc, key) }

#define SET_MEMBER_STRING(obj, json_doc, key, compulsory) \
{ if (json_doc.isMember(#key)) obj->key##_ = json_doc[#key].asString(); \
  else if (compulsory) THROW_ERROR(SET_MEMBER_STRING, json_doc, key) }

#define SET_MEMBER_DOUBLE_ASSTRING(obj, json_doc, key, compulsory) \
{ if (json_doc.isMember(#key)) { \
  std::stringstream ss; ss << json_doc[#key].asString(); ss >> obj->key##_; } \
  else if (compulsory) THROW_ERROR(SET_MEMBER_DOUBLE_ASSTRING, json_doc, key) }


struct HardwareVId {
  unsigned int Id_;
  std::string ShortName_;
  std::string FriendlyName_;

  void operator << (const Json::Value &fv_json);
};

struct FirmwareVId {
  unsigned int MajorId_;
  unsigned int MinorId_;
  std::string ShortName_;
  std::string FriendlyName_;

  void operator << (const Json::Value &fv_json);
};

struct MemoryLayout {
  MemoryLayout()
  : Index_(-1), Increment_(-1), MsbFirst_(false), Absolute_(false) {}
  int Index_;
  std::vector<int> AbsoluteIndexes_;
  int Increment_;
  bool MsbFirst_;
  bool Absolute_;

  void operator << (const Json::Value &v_json);
};

struct Variable {
  Variable() : BitActiveLow_(false) {}
  std::string Name_;
  std::string Type_;
  int Default_;
  int Value_;
  int Min_;
  int Max_;
  int BitSize_;
  bool BitActiveLow_;
  MemoryLayout MemoryLayout_;

  std::map<std::string, int> EnumValues_;

  void operator << (const Json::Value &v_json);
};

typedef  std::vector<Variable> Parameters;
Parameters& operator<<(Parameters &p, const Json::Value &v_json);
std::ostream & operator << (std::ostream &stream, const Parameters &par);

struct Channel {
  Parameters ChannelParameters_;
};

struct ASIC {
  std::vector <Channel> Channels_;
  Parameters PowerModesParameters_;
  Parameters GlobalControlParameters_;
};

struct FPGA {
  Parameters GlobalControlParameters_;
  std::vector<ASIC> Asics_;
};

struct Board {
  Parameters DirectParameters_;
  Parameters DataReadoutParameters_;
  Parameters StatusParameters_;

  void operator << (const Json::Value &p_json);
};




#endif

