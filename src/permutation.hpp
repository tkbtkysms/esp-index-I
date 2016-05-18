/* permutation.hpp
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

#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP

#include "full_fixed_length_code_array.hpp"
#include "RSDic.hpp"
#include "RSDicBuilder.hpp"

class Permutation{
public:
  Permutation (): elems_(),
		  has_skip_elem_(),
		  skip_list_() {}
  ~Permutation(){}
  void Build(FFLCArray elem, const uint64_t skip_length);
  uint64_t Access(const uint64_t kPos);
  uint64_t ReverseAccess(const uint64_t kPos);
  uint64_t Length();
  void Clear();
  void Save(std::ofstream &ofs);
  void Load(std::ifstream &ifs);
  uint64_t ByteSize(){
    return elems_.ByteSize() 
      + skip_list_.ByteSize()
      + has_skip_elem_.GetUsageBytes();
  }


private:
  FFLCArray elems_;
  rsdic::RSDic has_skip_elem_; 
  FFLCArray skip_list_;
};

#endif // PERMUTATION_HPP
