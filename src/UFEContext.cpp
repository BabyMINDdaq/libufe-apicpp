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
 *  \created  Dec 2016
 */

#include <iostream>
#include <fstream>

#include "UFEContext.h"
#include "UFEDevice.h"
#include "UFEDeff.h"
#include "UFEConfigFrame.h"

using namespace std;

UFEContext::UFEContext() : ufec_ctx_(nullptr) {
  ufe_default_context(&ufec_ctx_);
}

UFEContext* UFEContext::instance() {
  static UFEContext instance_;
  return &instance_;
}

void UFEContext::loadBMConfig(std::string file) {
  UFEConfigFrame::loadJsonFile(file, bm_config_doc_);
//   cout << bm_config_doc_ << std::endl;
  ufec_ctx_->verbose_             = bm_config_doc_["VerboseLevel"].asInt();
  ufec_ctx_->readout_timeout_     = bm_config_doc_["UsbParam"]["Timeout"].asInt();
  ufec_ctx_->readout_buffer_size_ = bm_config_doc_["UsbParam"]["PacketSize"].asInt();
}

void UFEContext::loadBMConfig() {
  string config_file(UFEAPI_DIR);
  config_file += "/conf/babyMIND-config.json";
  this->loadBMConfig(config_file);
}

void UFEContext::updateBMConfig(std::string file) {
  ofstream bm_config_file(file);
  bm_config_file << bm_config_doc_;
  bm_config_file.close();
}

void UFEContext::updateBMConfig() {
  string file_name(UFEAPI_DIR);
  file_name += "/conf/babyMIND-config.json";
  updateBMConfig(file_name);
}

