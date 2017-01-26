// C++
#include <unistd.h>
#include <fstream>
#include <mutex>
std::mutex dev_mtx_;

// libufecpp
#include "UFEDeff.h"
#include "UFEDevice.h"
#include "UFEError.h"
#include "UFEConfigFrame.h"
#include "UFEDataContainer.h"

#define GET_TIME std::chrono::high_resolution_clock::now()
#define GET_DURATION(t0) std::chrono::duration_cast<std::chrono::duration<double>>( \
std::chrono::high_resolution_clock::now()-t0).count()

typedef  std::chrono::high_resolution_clock::time_point  hd_time;

using namespace std;

int UFEDevice::init() {
  if (!device_) {
    throw UFEError( "Trying to initialize a dummy device.",
                    "void* UFEDevice::init()",
                    UFEError::FATAL);
  }

  status_ = ufe_open(device_, &handle_);
  if (status_ != 0) {
    stringstream ss;
    ss << "Device error, status is " << status_ <<".";
    throw UFEError( ss.str(),
                    "void* UFEDevice::init()",
                    UFEError::FATAL);
  }

  is_open_ = true;

  int xVerbose = ufe_get_context()->verbose_;
  ufe_get_context()->verbose_ = 0;
  for (int x_board=0; x_board<256; ++x_board) {
    if ( ufe_ping(handle_, x_board) )
      boards_.push_back(x_board);
  }

  ufe_get_context()->verbose_ = xVerbose;

  if (boards_.size()==0) {
    throw UFEError( "Corrupted Usb device",
                    "void* UFEDevice::init()",
                    UFEError::FATAL);
  }

  return boards_.size();
}

void UFEDevice::close() {
  ufe_close(handle_);
  boards_.resize(0);
  is_open_ = false;
}

void UFEDevice::start() {
  for (auto const &b: boards_) {
    status_ = ufe_data_readout(handle_, b, &readout_par_);
    if (status_ != 0) {
      stringstream ss;
      ss << "Error when loading the configuration on board " << b
         << " (status: " <<  status_ << ").";
      throw UFEError( ss.str(),
                      "int UFEDevice::start()",
                      UFEError::FATAL);
    }
  }
}

void UFEDevice::stop() {
  uint16_t data_16 = 0x0;
  for (auto const &b: boards_) {
    status_ = ufe_data_readout(handle_, b, &data_16);
    if (status_ != 0) {
      stringstream ss;
      ss << "Error when stoping board " << b
         << " (status: " <<  status_ << ").";
      throw UFEError( ss.str(),
                      "int UFEDevice::stop()",
                      UFEError::FATAL);
    }
  }
}

int UFEDevice::getData(UFEDataContainer* data_buffer, int *actual_read) {
  status_ = ufe_read_buffer( this->handle_,
                             data_buffer->buffer(),
                             actual_read);

  data_buffer->resize(*actual_read);

  // TODO This will work only for a single board (no daisy-chaining).
  // Has to be modified when the new firmware is available !!!
  data_buffer->setBoardId(this->boards_[0]);

  return status_;
}

void UFEDevice::readData(UFEDataContainer* data_buffer) {
  int actual_read(1);
  while (actual_read != 0) {
    this->getData(data_buffer, &actual_read);
    if (this->status_ != 0)
      break;
  }
}


void UFEDevice::readDataWithStats(UFEDataContainer* data_buffer) {
  int actual_read(1);
  hd_time t0 = GET_TIME;
  while (actual_read != 0) {
    this->getData(data_buffer, &actual_read);
    if (this->status_ != 0)
      break;

    read_tot_ += actual_read;
  }

  time_ += GET_DURATION(t0);
  {
    lock_guard<mutex> lock(dev_mtx_);
    cerr << "\ntime: " << time_ << "  data: " << read_tot_/(1024.*1024)
         << " MB   speed: " << (read_tot_/(1024*1024))/time_ << " MB/s\n\n";
  }
}

#define SIZE_CONFBUFF  36

void UFEDevice::configFromBinary(int boardId, string filename) {
  UFEConfigBuffer conf;
  ifstream stream(filename.c_str());
  if (!stream.is_open()) {
    stringstream ss;
    ss << "Error when opening file " << filename <<".";
    throw UFEError( ss.str(),
                    "int UFEDevice::configFromBinary(int, string)",
                    UFEError::SERIOUS);
  }

  for (int xDev=0; xDev<4; ++xDev) {
    conf.loadFromFile(stream);
//     conf.Dump();
    status_ = load_config(this->handle_, boardId, xDev, conf.getData(), conf.getSize());
    if (status_ != 0) {
      stringstream ss;
      ss << "Error when loading the configuration on board " << boardId
         << " (status: " <<  status_ << ").";
      throw UFEError( ss.str(),
                      "int UFEDevice::configFromBinary(int, string)",
                      UFEError::FATAL);
    }
  }

  uint16_t arg = 0xf; // Apply to all subdevices.
  status_ = ufe_apply_config(this->handle_, boardId, &arg);
  if (status_ != 0) {
    stringstream ss;
    ss << "Error when applying the configuration on board " << boardId
       << " (status: " <<  status_ << ").";
    throw UFEError( ss.str(),
                    "int UFEDevice::configFromBinary(int, string)",
                    UFEError::FATAL);
  }
}

void UFEDevice::configFromJson(int boardId, string file) {
  UFEConfigFrame frame;
  string config_descr_file(UFEAPI_DIR);
  config_descr_file += "/conf/config-desc.json";
  frame.loadConfigFrameFromJsonFile(config_descr_file);
  frame.loadReadoutParamFromJsonFile(file, this->readout_par_);

  frame.loadDirectParamFromJsonFile(file, this->direct_par_);
  this->setDirectParams();

  for (int xDev=0; xDev<4; ++xDev) {
    frame.loadUserConfigFromJsonFile(file, xDev);
    frame.setConfigBuffer(xDev);
    status_ = load_config(this->handle_, boardId, xDev, frame.getConfigData(), frame.getConfigDataSize());
    if (status_ != 0) {
      stringstream ss;
      ss << "Error when loading configuration on board " << boardId
         << " (status: " <<  status_ << ").";
      throw UFEError( ss.str(),
                      "int UFEDevice::configFromJson(int, string)",
                      UFEError::FATAL);
    }
  }

  uint16_t arg = 0x7; // Apply to ASICs 1/2/3.
  status_ = ufe_apply_config(this->handle_, boardId, &arg);
  if (status_ != 0) {
    stringstream ss;
    ss << "Error when applying configuration on board " << boardId
       << " (status: " <<  status_ << ").";
    throw UFEError( ss.str(),
                    "int UFEDevice::configFromJson(int, string)",
                    UFEError::FATAL);
  }

  arg = 0x8; // Apply to FPGA.
  status_ = ufe_apply_config(this->handle_, boardId, &arg);
  if (status_ != 0) {
    stringstream ss;
    ss << "Error when applying configuration on board " << boardId
       << " (status: " <<  status_ << ").";
    throw UFEError( ss.str(),
                    "int UFEDevice::configFromJson(int, string)",
                    UFEError::FATAL);
  }

  uint16_t data_16;
  status_ = ufe_read_status(this->handle_, boardId, &data_16);
  if (status_ != 0) {
    stringstream ss;
    ss << "Error when reading status on board " << boardId
       << " (status: " <<  status_ << ").";
    throw UFEError( ss.str(),
                    "int UFEDevice::configFromJson(int, string)",
                    UFEError::FATAL);
  }

  if (ufe_get_context()->verbose_ > 2) {
    ufe_debug_print("Status:");
    ufe_dump_status(data_16);
    ufe_debug_print("Readout params:");
    ufe_dump_readout_params(this->readout_par_);
  }
}

void UFEDevice::setDirectParams(int boardId, uint16_t par) {
  direct_par_ = par;
  setDirectParams(boardId);
}

void UFEDevice::setDirectParams(int boardId) {
  if (boardId < 0) {
    for (auto const & b: this->boards_) {
      status_ = ufe_set_direct_param(this->handle_, b, &this->direct_par_);
      if (status_)
        break;
    }
  } else {
    status_ = ufe_set_direct_param(this->handle_, boardId, &this->direct_par_);
  }

  if (status_ != 0) {
    stringstream ss;
    ss << "Error when setting direct params on board " << boardId
       << " (status: " <<  status_ << ").";
    throw UFEError( ss.str(),
                    "int UFEDevice::setDirectParams(int)",
                    UFEError::FATAL);
  }
}


std::string UFEDevice::boardsToString() {
  stringstream ss;
  ss << "(";
  int nBoards = boards_.size();
  if (nBoards==0)
    ss << "nan)";
  else {
    for (int i=0; i<nBoards-1; ++i)
      ss << boards_[i] << ", ";

    ss << boards_[nBoards-1] << ")";
  }

  return ss.str();
}





