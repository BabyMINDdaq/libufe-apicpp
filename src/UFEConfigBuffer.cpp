#include<iostream>
#include<sstream>
#include<fstream>
#include <math.h>
#include <bitset>

#include "UFEConfigBuffer.h"
#include "UFEError.h"

using namespace std;

UFEConfigBuffer::UFEConfigBuffer()
: actual_size_(0), buffer_(new uint32_t[size_]) {
  this->Reset();
}

void UFEConfigBuffer::LoadConfigFromTextFile(std::string file) {
    uint32_t *data_32 = buffer_;

    string line;
    stringstream ss;
    ss << hex;
    ifstream conf_file(file);

    if (conf_file) {
      while ( getline (conf_file, line) ) {
        ss << line;
        ss >> *data_32;
        data_32 ++;
      }
      conf_file.close();
    } else
      cout << "**  Error: cannot open file " << file << endl;
}

void UFEConfigBuffer::SetBitField(unsigned int index, unsigned int size, unsigned int value) {

  unsigned int max = (1 << size) - 1;
  if (value > max) {
    stringstream ss;
    ss << "Value exceeds the size limitation (" << value << ">" << max << ")";
    throw UFEError( ss.str(),
                    "UFEConfigBuffer::SetBitField(int, int, int)" ,
                    UFEError::FATAL);
  }

  if (index+size > size_*32) {
    stringstream ss;
    ss << "Value exceeds the size of the buffer (" << value << ">" << max << ")";
    throw UFEError( ss.str(),
                    "UFEConfigBuffer::SetBitField(int, int, int)" ,
                    UFEError::FATAL);
  }

  int pos = index/32;
  int shift = index%32;
  uint32_t *ptr = buffer_ + pos;

  if (index%32 + size < 32) {
    uint32_t mask, data;
    mask = ((1 << size ) - 1) << shift;
    data = value << shift;
    *ptr &= ~mask;
    *ptr |= data;
  } else {
    uint64_t *ptr_64 = (uint64_t*) ptr;
    uint64_t mask, data;
    mask = (uint64_t) ((1 << size ) - 1) << shift;
    data = (uint64_t) value << shift;
    *ptr_64 &= ~mask;
    *ptr_64 |= data;
  }
}

void UFEConfigBuffer::SetBitField(Variable var, char mode) {
int index;
  if (mode == 'n')
    index = var.MemoryLayout_.Index_;
  else if (mode == 'r')
    index = actual_size_ - var.MemoryLayout_.Index_ - var.BitSize_;
  else {
    stringstream ss;
    ss << "Unexpected mode " << mode << ".";
    throw UFEError( ss.str(),
                    "void UFEConfigBuffer::SetBitField(Variable, char)" ,
                    UFEError::FATAL);
  }

  int xValue = var.Value_;

  if (var.BitActiveLow_)
    xValue = ~xValue & ((1 << var.BitSize_) - 1);

  if ( (var.MemoryLayout_.MsbFirst_ && mode == 'n') ||
       (!var.MemoryLayout_.MsbFirst_ && mode == 'r') ) {
    xValue = this->Reverse(xValue, var.BitSize_);
  }

  this->SetBitField(index, var.BitSize_, xValue);
}

void UFEConfigBuffer::Reset() {
  for (unsigned int i=0; i<size_; ++i)
    buffer_[i] = 0;
}

void UFEConfigBuffer::Dump() {
  cout << hex;
  for (unsigned int i=0; i<size_; ++i)
    cout << "0x" << buffer_[i] << endl;

  cout << dec;
}

void UFEConfigBuffer::DumpToFile(ofstream &file) {
  file << hex;
  for (unsigned int i=0; i<size_; ++i)
    file << "0x" << buffer_[i] << endl;

  file << dec;
}

void UFEConfigBuffer::Print() {
  cout << hex;
  for (unsigned int i=0; i<size_; ++i) {
    cout << dec << i << "  " << std::bitset<32>(buffer_[i])
         << hex << "  0x" << buffer_[i]
         << "  index: " << dec << i*32 << " - " << (i+1)*32 << endl;
//     cout << dec << i << hex << " 0x" << buffer_[i] << endl;
  }

  cout << dec;
}

uint16_t UFEConfigBuffer::SetParams(const Parameters &par) {
  uint16_t buff(0);
  for (auto &p : par)
    buff |= (p.Value_& 0b1) << p.MemoryLayout_.Index_;

  return buff;
}

uint32_t UFEConfigBuffer::Reverse(int val, size_t size) {
  uint32_t reversed = 0;
  for (size_t i = 0; i < size; ++i) {
    reversed |= ((val>>i) & 1)<<(size-1-i);
  }

  return reversed;
}

void UFEConfigBuffer::Reverse(const size_t size) {
  uint8_t *buff_tmp = new uint8_t[size];
  uint8_t *buff_8 = (uint8_t*)buffer_;
  memcpy(buff_tmp, buff_8, size);

  this->Reset();
  for (unsigned int i=0; i<size; ++i)
    buff_8[i] = Reverse(buff_tmp[size-1-i], 8);

  delete[] buff_tmp;
}


