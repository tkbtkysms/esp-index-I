/* full_fixed_length_code_array.hpp
 * Copyright (C) 2015, Yoshimasa Takabatake, all rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 */

#ifndef FFLCARRAY_HPP
#define FFLCARRAY_HPP

#include <cmath>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>


static const uint64_t BITLOWERFIELD[] = {
  0x0000000000000000,
  0x0000000000000001, 
  0x0000000000000003,
  0x0000000000000007,
  0x000000000000000F,
  0x000000000000001F,
  0x000000000000003F,
  0x000000000000007F,
  0x00000000000000FF,
  0x00000000000001FF,
  0x00000000000003FF,
  0x00000000000007FF,
  0x0000000000000FFF,
  0x0000000000001FFF,
  0x0000000000003FFF,
  0x0000000000007FFF,
  0x000000000000FFFF,
  0x000000000001FFFF,
  0x000000000003FFFF,
  0x000000000007FFFF,
  0x00000000000FFFFF,
  0x00000000001FFFFF,
  0x00000000003FFFFF,
  0x00000000007FFFFF,
  0x0000000000FFFFFF,
  0x0000000001FFFFFF,
  0x0000000003FFFFFF,
  0x0000000007FFFFFF,
  0x000000000FFFFFFF,
  0x000000001FFFFFFF,
  0x000000003FFFFFFF,
  0x000000007FFFFFFF,
  0x00000000FFFFFFFF,
  0x00000001FFFFFFFF,
  0x00000003FFFFFFFF,
  0x00000007FFFFFFFF,
  0x0000000FFFFFFFFF,
  0x0000001FFFFFFFFF,
  0x0000003FFFFFFFFF,
  0x0000007FFFFFFFFF,
  0x000000FFFFFFFFFF,
  0x000001FFFFFFFFFF,
  0x000003FFFFFFFFFF,
  0x000007FFFFFFFFFF,
  0x00000FFFFFFFFFFF,
  0x00001FFFFFFFFFFF,
  0x00003FFFFFFFFFFF,
  0x00007FFFFFFFFFFF,
  0x0000FFFFFFFFFFFF,
  0x0001FFFFFFFFFFFF,
  0x0003FFFFFFFFFFFF,
  0x0007FFFFFFFFFFFF,
  0x000FFFFFFFFFFFFF,
  0x001FFFFFFFFFFFFF,
  0x003FFFFFFFFFFFFF,
  0x007FFFFFFFFFFFFF,
  0x00FFFFFFFFFFFFFF,
  0x01FFFFFFFFFFFFFF,
  0x03FFFFFFFFFFFFFF,
  0x07FFFFFFFFFFFFFF,
  0x0FFFFFFFFFFFFFFF,
  0x1FFFFFFFFFFFFFFF,
  0x3FFFFFFFFFFFFFFF,
  0x7FFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF,
};

static const uint64_t BITUPPERFIELD[] = {
  0x0000000000000000,
  0x8000000000000000, 
  0xC000000000000000,
  0xE000000000000000,
  0xF000000000000000,
  0xF800000000000000,
  0xFC00000000000000,
  0xFE00000000000000,
  0xFF00000000000000,
  0xFF80000000000000,
  0xFFC0000000000000,
  0xFFE0000000000000,
  0xFFF0000000000000,
  0xFFF8000000000000,
  0xFFFC000000000000,
  0xFFFE000000000000,
  0xFFFF000000000000,
  0xFFFF800000000000,
  0xFFFFC00000000000,
  0xFFFFE00000000000,
  0xFFFFF00000000000,
  0xFFFFF80000000000,
  0xFFFFFC0000000000,
  0xFFFFFE0000000000,
  0xFFFFFF0000000000,
  0xFFFFFF8000000000,
  0xFFFFFFC000000000,
  0xFFFFFFE000000000,
  0xFFFFFFF000000000,
  0xFFFFFFF800000000,
  0xFFFFFFFC00000000,
  0xFFFFFFFE00000000,
  0xFFFFFFFF00000000,
  0xFFFFFFFF80000000,
  0xFFFFFFFFC0000000,
  0xFFFFFFFFE0000000,
  0xFFFFFFFFF0000000,
  0xFFFFFFFFF8000000,
  0xFFFFFFFFFC000000,
  0xFFFFFFFFFE000000,
  0xFFFFFFFFFF000000,
  0xFFFFFFFFFF800000,
  0xFFFFFFFFFFC00000,
  0xFFFFFFFFFFE00000,
  0xFFFFFFFFFFF00000,
  0xFFFFFFFFFFF80000,
  0xFFFFFFFFFFFC0000,
  0xFFFFFFFFFFFE0000,
  0xFFFFFFFFFFFF0000,
  0xFFFFFFFFFFFF8000,
  0xFFFFFFFFFFFFC000,
  0xFFFFFFFFFFFFE000,
  0xFFFFFFFFFFFFF000,
  0xFFFFFFFFFFFFF800,
  0xFFFFFFFFFFFFFC00,
  0xFFFFFFFFFFFFFE00,
  0xFFFFFFFFFFFFFF00,
  0xFFFFFFFFFFFFFF80,
  0xFFFFFFFFFFFFFFC0,
  0xFFFFFFFFFFFFFFE0,
  0xFFFFFFFFFFFFFFF0,
  0xFFFFFFFFFFFFFFF8,
  0xFFFFFFFFFFFFFFFC,
  0xFFFFFFFFFFFFFFFE,
  0xFFFFFFFFFFFFFFFF,
};

class FullFixedLengthCodeArray{      
private:
  std::vector<uint64_t > data_;
  uint64_t length_;
  uint8_t bits_per_code_;

public:
  FullFixedLengthCodeArray() : data_(),
			       length_(),
			       bits_per_code_(){}
  ~FullFixedLengthCodeArray(){}

  void Init(const uint64_t kLength, const uint64_t kMaxVal);
  uint64_t length(){
    return length_;
  }
  
  inline void Set(const uint64_t kPos, 
		  const uint64_t kCode){
    
    size_t start_bit_pos = kPos*bits_per_code_;
    size_t start_array_pos =  floor((double)start_bit_pos/64);
    int8_t local_start_bit_pos = start_bit_pos - start_array_pos*64;
    int8_t shift_length = (64 - local_start_bit_pos - bits_per_code_); 

    if(shift_length >= 0){
      data_[start_array_pos] &= (BITLOWERFIELD[shift_length] | BITUPPERFIELD[local_start_bit_pos]);
      data_[start_array_pos] |= (kCode << shift_length);
    }	
    else{
      shift_length = -shift_length;
      data_[start_array_pos] &= BITUPPERFIELD[local_start_bit_pos];
      data_[start_array_pos] |= (kCode >> shift_length);
      data_[start_array_pos + 1] &= BITLOWERFIELD[64 - shift_length];
      data_[start_array_pos + 1] |= (kCode << (64 - shift_length));
    }
  }

  inline uint64_t Get(const uint64_t kPos){
    size_t start_bit_pos = kPos*bits_per_code_;
    size_t start_array_pos =  floor((double)start_bit_pos/64);
    int8_t local_start_bit_pos = start_bit_pos - start_array_pos*64;
    int8_t shift_length = (64 - local_start_bit_pos - bits_per_code_); 
     
    if(shift_length >= 0){
      return (data_[start_array_pos] & (BITLOWERFIELD[bits_per_code_] << shift_length)) >> shift_length;
    }
    else{
      uint64_t code = 0;
      shift_length = -shift_length;
      code = (data_[start_array_pos] & BITLOWERFIELD[bits_per_code_ - shift_length]) << shift_length;
      code += (data_[start_array_pos + 1] & BITUPPERFIELD[shift_length]) >> (64 - shift_length);
      return code;
    }
  }
 
  inline uint64_t operator[](const uint64_t kPos){
    return Get(kPos);
  }

  inline void Swap(const uint64_t kPos1,const uint64_t kPos2){
    uint64_t tmp_code = Get(kPos1);
    Set(kPos1,Get(kPos2));
    Set(kPos2,tmp_code);
  }

  uint64_t GetDataArrayLength();
  void Save(std::ofstream &ofs);
  void Load(std::ifstream &ifs);

  uint64_t ByteSize(){
    return sizeof(uint64_t)*GetDataArrayLength()
      + sizeof(uint64_t)
      + sizeof(uint8_t); 
  }
  
  void Clear(){
    length_ = 0;
    bits_per_code_ = 0;
    std::vector<uint64_t> ().swap(data_);
  }

};

typedef FullFixedLengthCodeArray FFLCArray;

#endif //FFLCARRAY_HPP
