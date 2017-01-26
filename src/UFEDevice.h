/** This file is part of BabyMINDdaq software package. This software
 * package is designed for internal use for the Baby MIND detector
 * collaboration and is tailored for this use primarily.
 *
 * BabyMINDdaq is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BabyMINDdaq is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BabyMINDdaq.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  \author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
 *            University of Geneva
 *
 *  \created  Nov 2016
 */

#ifndef UFE_DEVICE_H
#define UFE_DEVICE_H 1

// C++
#include <chrono>
#include <vector>
#include <jsoncpp/json/json.h>

// libufec
#include "libufe.h"
#include "libufe-tools.h"

// libufecpp
#include "UFEDataContainer.h"

class UFEDevice {
public:
  UFEDevice()
  : device_(nullptr), handle_(nullptr), readout_par_(0x580), direct_par_(0x8280),
    status_(0), time_(0), read_tot_(0) {}

  UFEDevice(libusb_device *dev)
  : device_(dev), handle_(nullptr), readout_par_(0x580), direct_par_(0x8280),
    status_(0), time_(0), read_tot_(0) {}

  virtual ~UFEDevice() {}

  int  init();
  void close();
  void start();
  void stop();

  void setDevice(libusb_device *dev) { device_ = dev;}
  void setReadoutPar(uint16_t p)     {readout_par_ = p;}
  bool isOpen() const { return is_open_;}
  int  getData(UFEDataContainer* data_buffer, int *actual_read);
  void readData(UFEDataContainer* data_buffer);
  void readDataWithStats(UFEDataContainer* data_buffer);

  void configFromBinary(int boardId, std::string file);
  void configFromJson(int boardId, std::string file);

  void setDirectParams(int boardId, uint16_t par);
  void setDirectParams(int boardId=-1);

  libusb_device         *device_;
  libusb_device_handle  *handle_;
  Json::Value      device_descr_;

  std::string boardsToString();

  std::vector<int> boards() const {return boards_;}
  std::vector<int>* boardsPtr()   {return &boards_;}
  int status()              const {return status_;}
  int read_tot()            const {return read_tot_;}
  bool is_open()            const {return is_open_;}

private:
  std::vector<int>   boards_;
  uint16_t readout_par_;
  uint16_t direct_par_;
  int status_;

  bool is_open_;
  double  time_;
  int read_tot_;

//   uint8_t data_buffer[BUF_SIZE];
};

#endif

