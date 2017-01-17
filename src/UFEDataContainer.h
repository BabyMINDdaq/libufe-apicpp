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

#ifndef UFE_DATA_CONTAINER_H
#define UFE_DATA_CONTAINER_H 1

// C++
#include <vector>

class UFEDataContainer {
public:
  UFEDataContainer();
  UFEDataContainer(size_t bsize);

  UFEDataContainer(const UFEDataContainer &c);
  UFEDataContainer(UFEDataContainer &&c);

  virtual ~UFEDataContainer();

  UFEDataContainer& operator=(const UFEDataContainer &c);
  UFEDataContainer& operator=(UFEDataContainer &&c);

  void append(const UFEDataContainer &c);
  void append(const UFEDataContainer &c, unsigned int pos, size_t size);
  UFEDataContainer& operator+=(const UFEDataContainer &c);

  void init(size_t bsize);

  void setBuffer(uint8_t* b);
  uint8_t* buffer() const {return buffer_;}

  size_t size()    const  {return size_;}
  size_t memsize() const  {return mem_size_;}

  void resize(size_t s)     {this->size_ = s;}
  void clear();

  int  boardId()   const  {return board_id_;}
  void setBoardId( int b) {board_id_ = b;}

  uint32_t* get32bWordPtr(unsigned int i=0);

  std::vector<uint64_t> spill_headers_;
  std::vector<uint64_t> spill_trailers_;

private:
  uint8_t  *buffer_;
  size_t    size_;
  size_t    mem_size_;
  int board_id_;

};

#endif

