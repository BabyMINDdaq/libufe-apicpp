#ifndef UFE_CONFIGBUFF_H
#define UFE_CONFIGBUFF_H 1

#include "UFEConfigLib.h"

class UFEConfigBuffer {
public:
  UFEConfigBuffer();
  virtual ~UFEConfigBuffer() {delete[] buffer_;}

  uint32_t* getData()    const { return buffer_;}
  unsigned int getSize() const {return size_;}

  void setActualSize(int s) {actual_size_ = s;}
  void loadConfigFromTextFile(std::string file);
  void reset();
  void dump();
  void dumpToFile(std::ofstream &file);
  void loadFromFile(std::ifstream &file);
  void print();
  void reverseDump();

  void setBitField(unsigned int index, unsigned int size, unsigned int value);
  void setBitField(Variable var, char mode='n');
  uint16_t setParams(const Parameters &par);

  uint32_t reverse(int val, const size_t size);
  void     reverse(const size_t size);

private:
  static const unsigned int size_ = 36;
  unsigned int actual_size_;
  uint32_t* buffer_;
};

#endif



