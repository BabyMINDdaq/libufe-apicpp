#ifndef UFE_CONFIGBUFF_H
#define UFE_CONFIGBUFF_H 1

#include "UFEConfigLib.h"

class UFEConfigBuffer {
public:
  UFEConfigBuffer();
  virtual ~UFEConfigBuffer() {delete[] buffer_;}

  uint32_t* GetData() { return buffer_;}
  unsigned int GetSize() {return size_;}

  void SetActualSize(int s) {actual_size_ = s;}
  void LoadConfigFromTextFile(std::string file);
  void Reset();
  void Dump();
  void DumpToFile(std::ofstream &file);
  void Print();
  void ReverseDump();

  void SetBitField(unsigned int index, unsigned int size, unsigned int value);
  void SetBitField(Variable var, char mode='n');
  uint16_t SetParams(const Parameters &par);

  uint32_t Reverse(int val, const size_t size);
  void     Reverse(const size_t size);

private:
  static const unsigned int size_ = 36;
  unsigned int actual_size_;
  uint32_t* buffer_;
};

#endif



