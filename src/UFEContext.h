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

#ifndef UFE_CONTEXT_H
#define UFE_CONTEXT_H 1

// C++
#include <jsoncpp/json/json.h>

// libufec
#include "libufe.h"

// libufecpp
#include "UFEDevice.h"
// class UFEDevice;

class UFEContext {
 public:
  static UFEContext* instance();

  ufe_context *ufec_ctx_;
  libusb_device **febs_;
  std::vector<UFEDevice> devs_;
  Json::Value bm_config_doc_;

  void loadBMConfig();
  void loadBMConfig(std::string file);

  void updateBMConfig();
  void updateBMConfig(std::string file);

private:
  UFEContext();
  virtual ~UFEContext(){};
};

#endif


