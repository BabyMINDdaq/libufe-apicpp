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


#ifndef UEF_DATARECORDER_H
#define UEF_DATARECORDER_H 1

// C++
#include <fstream>
#include <string>

#include "UFEDataContainer.h"

class UFEDataRecorder {
 public:

  UFEDataRecorder();
  virtual ~UFEDataRecorder();

  void   setRunNum(int n)           {run_number_ = n;}
  void   setDataPath(std::string p);
  void   seMaxFileSize(int s) {max_file_size_ = s;}

  int    getRunNum()       const {return run_number_;}
  size_t getDataSize()     const {return data_size_;}

  void init();
  void write(UFEDataContainer *data);
  void close();
private:
  void openNewFile();

  std::ofstream out_file_;
  int           run_number_;
  std::string   data_path_;
  size_t        data_size_;
  size_t        file_size_;
  size_t        max_file_size_;
  int           file_number_;
  std::string   file_name_;
};

#endif

