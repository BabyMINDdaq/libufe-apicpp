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

// C++
#include <sstream>

// libufecpp
#include "UFEDataContainer.h"
#include "UFEContext.h"
#include "UFEError.h"

using namespace std;

UFEDataContainer::UFEDataContainer()
: buffer_(nullptr), size_(0), mem_size_(0), board_id_(-1) {
  this->mem_size_ = UFEContext::instance()->ufec_ctx_->readout_buffer_size_;
  this->buffer_ = new uint8_t[this->mem_size_];
}

UFEDataContainer::UFEDataContainer(size_t bsize)
: buffer_(nullptr), size_(0), mem_size_(bsize), board_id_(-1) {
  if (bsize > 0)
    this->buffer_ = new uint8_t[bsize];
}

UFEDataContainer::UFEDataContainer(const UFEDataContainer &c)
: buffer_(nullptr), size_(0), mem_size_(0), board_id_(-1) {
  *this = c;
}

UFEDataContainer::UFEDataContainer(UFEDataContainer &&c)
: buffer_(nullptr), size_(0), mem_size_(0), board_id_(-1) {
  *this = std::move(c);
}

UFEDataContainer::~UFEDataContainer() {
  if (this->mem_size_ != 0)
    delete[] buffer_;
}

UFEDataContainer& UFEDataContainer::operator=(const UFEDataContainer &c) {
  if (this != &c) {
    if ( this->mem_size_ != c.memsize() ) {
      if (this->buffer_ != nullptr)
        delete[] this->buffer_;

      this->buffer_ = new uint8_t[c.memsize()];
      this->mem_size_ = c.memsize();
    }

    this->size_ = c.size();
    memcpy(this->buffer_, c.buffer_, c.memsize());

    this->spill_headers_  = c.spill_headers_;
    this->spill_trailers_ = c.spill_trailers_;
  }

  return *this;
}

UFEDataContainer& UFEDataContainer::operator=(UFEDataContainer &&c) {
  if (this != &c) {
    if (this->buffer_ != nullptr)
      delete[] this->buffer_;

    this->buffer_   = c.buffer_;
    this->size_     = c.size();
    this->mem_size_ = c.memsize();

    this->spill_headers_  = std::move(c.spill_headers_);
    this->spill_trailers_ = std::move(c.spill_trailers_);

    c.buffer_ = nullptr;
    c.resize(0);
    c.mem_size_= 0;
}

  return *this;
}

// UFEDataContainer& UFEDataContainer::operator+(const UFEDataContainer &c) {
//   
//   return *this;
// }

void UFEDataContainer::append(const uint8_t *data, size_t s) {
  if ( (this->size_ + s) < this->mem_size_) {
    memcpy(this->buffer_+this->size_, data, s);
  } else {
    uint8_t *new_buffer = new uint8_t[this->size_+s];
    this->mem_size_ = this->size_ + s;

    memcpy(new_buffer, this->buffer_, this->size_);
    memcpy(new_buffer+this->size_, data, s);

    delete[] this->buffer_;
    this->buffer_ = new_buffer;
  }

  this->size_ += s;
}

void UFEDataContainer::append(const UFEDataContainer &c) {
  this->append(c, 0, c.size());
}

void UFEDataContainer::append(const UFEDataContainer &c, unsigned int pos, size_t size) {
  if ((pos+size)>c.size()) {
    stringstream ss;
    ss << "Size exceeded (" << pos+size << " > " << c.size_ << ").";
    throw UFEError( ss.str(),
                    "uint32_t* UFEDataContainer::append(UFEDataContainer, unsigned int, size_t)",
                    UFEError::FATAL);
  }

  for (auto const & h: c.spill_headers_) {
    if (h>=pos && h <pos+size)
      this->spill_headers_.push_back(this->size_+h-pos);
  }

  for (auto const & t: c.spill_trailers_) {
    if (t>=pos && t <pos+size)
      this->spill_trailers_.push_back(this->size_+t-pos);
  }

  this->append(c.buffer_+pos, size);
}

UFEDataContainer& UFEDataContainer::operator+=(const UFEDataContainer &c) {
  this->append(c);
  return *this;
}


void UFEDataContainer::setBuffer(uint8_t* b) {
  if (buffer_ != nullptr)
    delete[] this->buffer_;

  this->buffer_ = b;
}

void UFEDataContainer::clear() {
  this->resize(0);
  spill_headers_.resize(0);
  spill_trailers_.resize(0);
  board_id_ = -1;
}

uint32_t* UFEDataContainer::get32bWordPtr(unsigned int i) {
  if ( (i*4) < this->size_ )
    return (uint32_t*) &buffer_[i*4];
  else {
    stringstream ss;
    ss << "Size exceeded (i=" << i << " size=" << this->size_ << ").";
    throw UFEError( ss.str(),
                    "uint32_t* UFEDataContainer::get32bWordPtr(unsigned int)",
                    UFEError::FATAL);
  }
}