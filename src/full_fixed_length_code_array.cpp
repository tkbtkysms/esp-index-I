/* full_fixed_length_code_array.cpp
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

#include "full_fixed_length_code_array.hpp"

using namespace std;

void FFLCArray::Init(const uint64_t kLength, const uint64_t kMaxVal){
  bits_per_code_ = floor(log2(kMaxVal)) + 1;
  if(bits_per_code_ == 0){
    bits_per_code_ = 1;
  }
  length_ = kLength;
  data_.resize(GetDataArrayLength());
  vector<uint64_t> (data_).swap(data_);
  for(size_t i = 0; i < GetDataArrayLength();i++){
    data_[i] = 0;
  }
}

uint64_t FFLCArray::GetDataArrayLength(){
  return floor((double)(length_ * bits_per_code_) / 64) + 1;
}

void FFLCArray::Save(ofstream &ofs){
  ofs.write((char*)&length_, sizeof(uint64_t));
  ofs.write((char*)&bits_per_code_, sizeof(uint8_t));
  ofs.write((char*)&data_[0], GetDataArrayLength()*sizeof(uint64_t));
}

void FFLCArray::Load(ifstream &ifs){
  ifs.read((char*)&length_, sizeof(uint64_t));
  ifs.read((char*)&bits_per_code_, sizeof(uint8_t));
  data_.resize(GetDataArrayLength());
  ifs.read((char*)&data_[0], GetDataArrayLength() * sizeof(uint64_t));
}

