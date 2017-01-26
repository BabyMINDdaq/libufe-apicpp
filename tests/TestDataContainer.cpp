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

#include <iostream>

#include "UFEContext.h"
#include "UFEError.h"

#include "TestDataContainer.h"

using namespace std;

void TestDataContainer::setUp() {
  
}

void TestDataContainer::tearDown() {
  
}

void TestDataContainer::TestInit() {
  UFEDataContainer c1;
  ufe_context *ctx = UFEContext::instance()->ufec_ctx_;
  unsigned int size = ctx->readout_buffer_size_;
  CPPUNIT_ASSERT( c1.memsize() == size );
  CPPUNIT_ASSERT( c1.size() == 0 );

  int testVal = c1.buffer()[size-1];

  UFEDataContainer c2(32);
  CPPUNIT_ASSERT( c2.memsize() == 32 );
  CPPUNIT_ASSERT( c2.size() == 0 );
  testVal = c1.buffer()[31];

  CPPUNIT_ASSERT(&testVal);

  ctx->readout_buffer_size_ = 16;
  UFEDataContainer c3;
  CPPUNIT_ASSERT( c3.memsize() == 16 );

  UFEDataContainer c4(0);
  CPPUNIT_ASSERT( c4.memsize() == 0 );
  CPPUNIT_ASSERT( c4.size() == 0 );
  CPPUNIT_ASSERT( c4.buffer() == nullptr );
}

void TestDataContainer::TestCopy() {
  UFEDataContainer c5(8);
  uint8_t *d = new uint8_t[8];
   for (int i=0; i<6; ++i) {
    d[i] = i+1;
  }
  c5.setBuffer( d );
  c5.resize(6);

  for (int i=0; i<6; ++i) {
    CPPUNIT_ASSERT( (c5.buffer())[i] == i+1 );
  }

  UFEDataContainer c6(16);
  CPPUNIT_ASSERT( c6.memsize() == 16 );
  CPPUNIT_ASSERT( c6.size() == 0 );
  c6 = c5;
  CPPUNIT_ASSERT( c6.memsize() == 8 );
  CPPUNIT_ASSERT( c6.size() == 6 );

  for (int i=0; i<6; ++i) {
    CPPUNIT_ASSERT( c5.buffer()[i] == i+1 );
    c5.buffer()[i] = 0;
    CPPUNIT_ASSERT( (c6.buffer())[i] == i+1 );
  }

  UFEDataContainer c7(c6);
  CPPUNIT_ASSERT( c7.memsize() == 8 );
  CPPUNIT_ASSERT( c7.size() == 6 );

  for (int i=0; i<6; ++i) {
    CPPUNIT_ASSERT( (c7.buffer())[i] == i+1 );
    CPPUNIT_ASSERT( (c6.buffer())[i] == i+1 );
  }
}

void TestDataContainer::TestMove() {
  UFEDataContainer c8(8);
  uint8_t *d = new uint8_t[8];
  for (int i=0; i<7; ++i) {
    d[i] = i+4;
  }

  c8.setBuffer( d );
  c8.resize(7);

  c8.spill_headers_.push_back(1);
  c8.spill_headers_.push_back(10);
  c8.spill_trailers_.push_back(7);

  CPPUNIT_ASSERT( c8.memsize() == 8 );
  CPPUNIT_ASSERT( c8.size() == 7 );
  CPPUNIT_ASSERT( c8.buffer() == d );

  for (int i=0; i<7; ++i) {
    CPPUNIT_ASSERT( c8.buffer()[i] == i+4 );
  }

  UFEDataContainer c9(std::move(c8));
  CPPUNIT_ASSERT( c8.memsize() == 0 );
  CPPUNIT_ASSERT( c8.buffer() == nullptr );
  CPPUNIT_ASSERT( c8.size() == 0 );
  CPPUNIT_ASSERT( c9.memsize() == 8 );
  CPPUNIT_ASSERT( c9.size() == 7 );
  CPPUNIT_ASSERT( c9.buffer() == d );
  CPPUNIT_ASSERT(c8.spill_headers_.size() == 0);
  CPPUNIT_ASSERT(c8.spill_trailers_.size() == 0);
  CPPUNIT_ASSERT(c9.spill_headers_.size() == 2);
  CPPUNIT_ASSERT(c9.spill_trailers_.size() == 1);

  for (int i=0; i<7; ++i) {
    CPPUNIT_ASSERT( (c9.buffer())[i] == i+4 );
  }

  UFEDataContainer c10;
  c10 = std::move(c9);
  CPPUNIT_ASSERT( c9.memsize() == 0 );
  CPPUNIT_ASSERT( c9.buffer() == nullptr );
  CPPUNIT_ASSERT( c9.size() == 0 );
  CPPUNIT_ASSERT( c10.memsize() == 8 );
  CPPUNIT_ASSERT( c10.size() == 7 );
  CPPUNIT_ASSERT( c10.buffer() == d );

  for (int i=0; i<7; ++i) {
    CPPUNIT_ASSERT( (c10.buffer())[i] == i+4 );
  }

  c10 = std::move(c9);
  CPPUNIT_ASSERT( c10.memsize() == 0 );
  CPPUNIT_ASSERT( c10.size() == 0 );
  CPPUNIT_ASSERT( c10.buffer() == nullptr );
}

void TestDataContainer::TestAdd() {
  UFEDataContainer c11(18);
  uint8_t *ptr = c11.buffer();
  for (int i=0; i<7; ++i) {
    c11.buffer()[i] = i+8;
  }
  c11.resize(7);

  for (int i=0; i<7; ++i) {
    CPPUNIT_ASSERT( (c11.buffer())[i] == i+8 );
  }

  UFEDataContainer c12(16);
  for (int i=0; i<6; ++i) {
    c12.buffer()[i] = i+3;
  }
  c12.resize(6);

  for (int i=0; i<6; ++i) {
    CPPUNIT_ASSERT( (c12.buffer())[i] == i+3 );
  }

  c11 += c12;
  CPPUNIT_ASSERT( c11.size() == 6+7 );
  CPPUNIT_ASSERT( c11.memsize() == 18 );
  CPPUNIT_ASSERT( c11.buffer() == ptr );

  for (int i=0; i<7; ++i) {
    CPPUNIT_ASSERT( (c11.buffer())[i] == i+8 );
  }

  for (int i=0; i<6; ++i) {
    CPPUNIT_ASSERT( (c11.buffer())[i+7] == i+3 );
  }

  UFEDataContainer c13(27);
  for (int i=0; i<9; ++i) {
    c13.buffer()[i] = i+5;
  }
  c13.resize(9);

  for (int i=0; i<9; ++i) {
    CPPUNIT_ASSERT( (c13.buffer())[i] == i+5 );
  }

  c11 += c13;
  CPPUNIT_ASSERT( c11.size() == 6+7+9 );
  CPPUNIT_ASSERT( c11.memsize() == 6+7+9 );
  CPPUNIT_ASSERT( c11.buffer() != ptr );

  for (int i=0; i<7; ++i) {
    CPPUNIT_ASSERT( (c11.buffer())[i] == i+8 );
  }

  for (int i=0; i<6; ++i) {
    CPPUNIT_ASSERT( (c11.buffer())[i+7] == i+3 );
  }

  for (int i=0; i<9; ++i) {
    CPPUNIT_ASSERT( (c11.buffer())[i+7+6] == i+5 );
  }

  UFEDataContainer c16(26), c17(26);
  for (int i=0; i<26; ++i) {
    c16.buffer()[i] = i;
    c17.buffer()[i] = i;
  }
  c16.resize(12);
  c17.resize(7);

  for (int i=0; i<26; ++i) {
    CPPUNIT_ASSERT( (c16.buffer())[i] == i );
    CPPUNIT_ASSERT( (c17.buffer())[i] == i );
  }

  int pos=2;
  c16.append(c17, pos, 3);
  CPPUNIT_ASSERT( c16.size() == 12+3 );

  c16.append(c17, 0, 4);
  CPPUNIT_ASSERT( c16.size() == 12+3+4 );

  for (int i=0; i<12; ++i) {
    CPPUNIT_ASSERT( (c16.buffer())[i] == i );
  }
  for (int i=12; i<15; ++i) {
    CPPUNIT_ASSERT( (c16.buffer())[i] == i-12+2 );
  }
  for (int i=15; i<19; ++i) {
    CPPUNIT_ASSERT( (c16.buffer())[i] == i-15 );
  }
  for (int i=19; i<26; ++i) {
//     cerr << i << ": " <<  (int) (c16.buffer())[i] << endl;
    CPPUNIT_ASSERT( (c16.buffer())[i] == i );
  }

  UFEDataContainer c18(60), c19(60);
  for (int i=0; i<60; ++i) {
    c18.buffer()[i] = i;
    c19.buffer()[i] = i;
  }

  c18.resize(50);
  c19.resize(47);

  c18.buffer()[8] = 0xff;
  c18.spill_headers_.push_back(8);

  c18.buffer()[36] = 0xee;
  c18.spill_trailers_.push_back(36);

  c18.buffer()[39] = 0xff;
  c18.spill_headers_.push_back(39);


  c19.buffer()[3] = 0xee;
  c19.spill_trailers_.push_back(3);

  c19.buffer()[10] = 0xff;
  c19.spill_headers_.push_back(10);

  c19.buffer()[23] = 0xee;
  c19.spill_trailers_.push_back(23);

  c19.buffer()[30] = 0xff;
  c19.spill_headers_.push_back(30);


  c19.buffer()[43] = 0xee;
  c19.spill_trailers_.push_back(43);

  c19.buffer()[50] = 0xff;
  c19.spill_headers_.push_back(50);

  c18.append(c19);


  CPPUNIT_ASSERT(c18.spill_headers_.size() == 4);
  CPPUNIT_ASSERT(c18.spill_headers_[0] == 8);
  CPPUNIT_ASSERT(c18.spill_headers_[1] == 39);
  CPPUNIT_ASSERT(c18.spill_headers_[2] == 50+10);
  CPPUNIT_ASSERT(c18.spill_headers_[3] == 50+30);

  CPPUNIT_ASSERT(c18.spill_trailers_.size() == 4);
  CPPUNIT_ASSERT(c18.spill_trailers_[0] == 36);
  CPPUNIT_ASSERT(c18.spill_trailers_[1] == 50+3);
  CPPUNIT_ASSERT(c18.spill_trailers_[2] == 50+23);
  CPPUNIT_ASSERT(c18.spill_trailers_[3] == 50+43);

  CPPUNIT_ASSERT(c18.size() == 50+47);
  CPPUNIT_ASSERT(c18.memsize() == 50+47);

  c18.append(c19, 6, 20);

  CPPUNIT_ASSERT(c18.spill_headers_.size() == 5);
  CPPUNIT_ASSERT(c18.spill_headers_[0] == 8);
  CPPUNIT_ASSERT(c18.spill_headers_[1] == 39);
  CPPUNIT_ASSERT(c18.spill_headers_[2] == 50+10);
  CPPUNIT_ASSERT(c18.spill_headers_[3] == 50+30);
  CPPUNIT_ASSERT(c18.spill_headers_[4] == 50+47+10-6);

  CPPUNIT_ASSERT(c18.spill_trailers_.size() == 5);
  CPPUNIT_ASSERT(c18.spill_trailers_[0] == 36);
  CPPUNIT_ASSERT(c18.spill_trailers_[1] == 50+3);
  CPPUNIT_ASSERT(c18.spill_trailers_[2] == 50+23);
  CPPUNIT_ASSERT(c18.spill_trailers_[3] == 50+43);
  CPPUNIT_ASSERT(c18.spill_trailers_[4] == 50+47+23-6);

  CPPUNIT_ASSERT(c18.size() == 117);
  CPPUNIT_ASSERT(c18.memsize() == 117);

  for (auto const& h: c18.spill_headers_)
    CPPUNIT_ASSERT( c18.buffer()[h] == 0xff );
//     cerr << h << ": " << hex << (int) c18.buffer()[h] << dec << endl;

  for (auto const& t: c18.spill_trailers_)
    CPPUNIT_ASSERT( c18.buffer()[t] == 0xee);
//     cerr << t << ": " << hex << (int) c18.buffer()[t] << dec << endl;
}

void TestDataContainer::TestClear() {
  UFEDataContainer c14(16);
  for (int i=0; i<12; ++i) {
    c14.buffer()[i] = i+3;
  }
  c14.resize(12);

  c14.spill_headers_.push_back(1);
  c14.spill_headers_.push_back(10);

  c14.spill_trailers_.push_back(7);

  c14.clear();
  CPPUNIT_ASSERT( c14.size() == 0 );
  CPPUNIT_ASSERT( c14.memsize() == 16 );
  CPPUNIT_ASSERT( c14.spill_headers_.size() == 0 );
  CPPUNIT_ASSERT( c14.spill_trailers_.size() == 0 );
}

void TestDataContainer::TestGet32Ptr() {
  UFEDataContainer c15(16);
  c15.buffer()[0] = 0xaa;
  c15.buffer()[1] = 0xbb;
  c15.buffer()[2] = 0xcc;
  c15.buffer()[3] = 0xdd;
  c15.resize(4);
  uint32_t *ptr = c15.get32bWordPtr(0);
  CPPUNIT_ASSERT( *ptr = 0xddccbbaa );

  c15.buffer()[8] = 0x01;
  c15.buffer()[9] = 0xef;
  c15.buffer()[10] = 0xcd;
  c15.buffer()[11] = 0xab;
  c15.resize(12);
  ptr = c15.get32bWordPtr(2);
  CPPUNIT_ASSERT( *ptr = 0xabcdef01 );

  bool excetion(false);

  try {
    ptr = c15.get32bWordPtr(3);
  } catch (UFEError &e) {
    excetion = true;
  }

  CPPUNIT_ASSERT( excetion = true );

  excetion = false;

    try {
    ptr = c15.get32bWordPtr(333);
  } catch (UFEError &e) {
    excetion = true;
  }

  CPPUNIT_ASSERT( excetion = true );
}

