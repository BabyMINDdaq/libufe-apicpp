#include <algorithm>

#include "UFEConfigLib.h"
#include "UFEError.h"

using namespace std;

void HardwareVId::operator << (const Json::Value &fv_json) {
  SET_MEMBER_INT(this, fv_json, Id, 1)
  SET_MEMBER_STRING(this, fv_json, ShortName, 1)
  SET_MEMBER_STRING(this, fv_json, FriendlyName, 1)
}

void FirmwareVId::operator << (const Json::Value &fv_json) {
  SET_MEMBER_INT(this, fv_json, MajorId, 1)
  SET_MEMBER_INT(this, fv_json, MinorId, 1)
  SET_MEMBER_STRING(this, fv_json, ShortName, 1)
  SET_MEMBER_STRING(this, fv_json, FriendlyName, 1)
}

void Variable::operator << (const Json::Value &v_json) {
  if (v_json.size()) {
    if (v_json.isMember("$type")) {
      string type = v_json["$type"].asString();
      if (type != "UnigeFrontEnd.Config.DescVariable") {
        stringstream ss;
        ss << v_json << endl;
        ss << "*** This is not a Variable!";
        throw UFEError( ss.str(),
                        "void Variable::operator << (const Json::Value&)" ,
                        UFEError::FATAL);
      }

      try {
        SET_MEMBER_INT(this, v_json, Default, 1)
      } catch (std::runtime_error) {
//         cout << v_json["EnumValues"] << endl;
        for (auto const& e : v_json["EnumValues"].getMemberNames()) {
          this->EnumValues_[e] = v_json["EnumValues"][e].asInt();
        }

        this->Default_ = EnumValues_[ v_json["Default"].asString() ];
      }
      this->Value_ = this->Default_;

      SET_MEMBER_INT(this, v_json, Min, 1)
      SET_MEMBER_INT(this, v_json, Max, 1)
      SET_MEMBER_INT(this, v_json, BitSize, 1)
      SET_MEMBER_STRING(this, v_json, Name, 1)
      SET_MEMBER_STRING(this, v_json, Type, 1)
      SET_MEMBER_BOOL(this, v_json, BitActiveLow, 0);

      this->MemoryLayout_ << v_json["MemoryLayout"];
    }
  } else {
//     cout << this->Name_ << "  " << this->Value_ << " " << v_json << endl;
    if (this->Type_ == "Bool" && v_json.isConvertibleTo(Json::booleanValue)) {
      if (this->Value_ != v_json.asBool()) {
//         cout << "Reseting " << this->Name_ << ": " << this->Value_ << "->" << v_json.asBool() << endl;
      }
      this->Value_ = v_json.asBool();
      return;
    } else if (v_json.isConvertibleTo(Json::uintValue)) { 
      int xVal = v_json.asInt();
      if (xVal >= this->Min_ && xVal <= this->Max_) {
        this->Value_ = v_json.asInt();
        return;
      }
    } else if (v_json.isConvertibleTo(Json::stringValue)) {
      if (EnumValues_.find(v_json.asString()) != EnumValues_.end()) {
        this->Value_ = EnumValues_[v_json.asString()];
//         cout << v_json.asString() << ": " << this->Value_ << endl;
        return;
      }
    }

    stringstream ss;
    string var = v_json.toStyledString();
    var.erase(std::remove(var.begin(), var.end(), '\n'), var.end());
    ss << "Unexpected type or value of the variable " << this->Name_ << ": " << var;
    throw UFEError( ss.str(),
                    "void Variable::operator << (const Json::Value&)" ,
                    UFEError::FATAL);
  }
}

void MemoryLayout::operator << (const Json::Value &ml_json) {
  try {
    SET_MEMBER_INT(this, ml_json, Index, 1)
  } catch (UFEError e) {
    if ( !ml_json.isMember("AbsoluteIndexes") )
      throw e;

    if ( !ml_json["AbsoluteIndexes"].isArray() )
      throw e;

    for (auto const & i : ml_json["AbsoluteIndexes"])
      AbsoluteIndexes_.push_back(i.asInt());
  }

  SET_MEMBER_INT(this, ml_json, Increment, 0)
  SET_MEMBER_BOOL(this, ml_json, MsbFirst, 0)
  SET_MEMBER_BOOL(this, ml_json, Absolute, 0)
}

void Board::operator << (const Json::Value &b_json) {
//   cout << b_json << endl;
  if ( b_json.isMember("$type") &&
       b_json.isMember("Name")  &&
       b_json.isMember("Children") ) {
    string type = b_json["$type"].asString();
    string name = b_json["Name"].asString();
    if ( type == "UnigeFrontEnd.Config.DataArray" ||
         name == "Board") {
      if (b_json["Children"].isArray()) {
        for (auto const& par : b_json["Children"]) {
          if (par["Name"] == "DirectParam")
           this->DirectParameters_ << par;
        }

        return;
      }
    }
  }

  stringstream ss;
  ss << b_json << endl;
  ss << "*** Unexpected structure of the user configuration.!";
  throw UFEError( ss.str(),
                  "void Board::operator << (const Json::Value&)",
                  UFEError::FATAL);
}

Parameters& operator<<(Parameters &p, const Json::Value &p_json) {
//   cout << p_json << endl;
  if (p_json.isMember("$type")) {
    string type = p_json["$type"].asString();
    if ( type == "UnigeFrontEnd.Config.DataVarList" &&
         p_json.isMember("Children")) {
      for (auto &v : p) {
        string name =  v.Name_;
        if (p_json["Children"].isMember(name))
          v << p_json["Children"][name];
      }

      return p;
    }
  }

  stringstream ss;
  ss << p_json << endl;
  ss << "*** Unexpected structure of the user configuration.!";
  throw UFEError( ss.str(),
                  "Parameters& operator<<(Parameters&, const Json::Value&)",
                  UFEError::FATAL);
}

ostream & operator << (ostream &stream, const Parameters &par) {
  for (auto &p : par)
    stream << p.Name_ << ": " << p.Value_ << endl;
  return stream;
}




