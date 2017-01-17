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
 *  \created  Jan 2017
 */


#include <sstream>
#include <iomanip>

#include "UFEDataRecorder.h"
#include "UFEContext.h"
#include "UFEError.h"

using namespace std;

UFEDataRecorder::UFEDataRecorder()
: run_number_(0), data_size_(0), file_size_(0), max_file_size_(1024*1024*250),
  file_number_(0), file_name_("0.000") {}

UFEDataRecorder::~UFEDataRecorder() {
  if (out_file_.is_open()) {
    out_file_.close();
    remove(file_name_.c_str());
  }
}

void UFEDataRecorder::init() {
  UFEContext *ctx = UFEContext::instance();
  run_number_ = ctx->bm_config_doc_["RunNumber"].asInt();
  this->openNewFile();
}

void UFEDataRecorder::write(UFEDataContainer *data) {
  out_file_.write( (char*) data->buffer(), data->size());
  data_size_ += data->size();
  file_size_ += data->size();
  data->clear();

  if (file_size_ > max_file_size_) {
    out_file_.close();
    ++file_number_;
    this->openNewFile();
    file_size_ = 0;
  }
}

void UFEDataRecorder::close() {
  cerr << "UFEDataRecorder::close\n";
  out_file_.close();
  run_number_++;
  UFEContext *ctx = UFEContext::instance();
  ctx->bm_config_doc_["RunNumber"] = run_number_;
}

void UFEDataRecorder::openNewFile() {
  stringstream ss;
  ss << data_path_ << run_number_ << "." ;
  ss << setfill('0') << setw (3);
  ss << file_number_;
  file_name_ = ss.str();
  out_file_.open(file_name_, ofstream::binary);
  if ( !out_file_.is_open() ) {
    stringstream ss;
    ss << "Error opening file " << file_name_ <<".";
    throw UFEError( ss.str(),
                    "void UFEDataRecorder::openFile()",
                    UFEError::FATAL);
  }
}

void UFEDataRecorder::setDataPath(string p) {
  data_path_ = p;
  if (p.back() != '/') data_path_ += '/';
}


