/* gmr.hpp
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

#ifndef GMR_HPP
#define GMR_HPP

#include "RSDic.hpp"
#include "RSDicBuilder.hpp"
#include "constant_numbers.hpp"
#include "permutation.hpp"

class GMR{
public:
  GMR(): b_(), perm_(), max_var_(){}
  ~GMR() {}
  void     Build (FFLCArray &fflca,   const uint64_t kPermSkipInterval);
  uint64_t Access(const uint64_t kPos);
  uint64_t Rank  (const uint64_t kVar, const uint64_t kPos);
  uint64_t Select(const uint64_t kVar, const uint64_t kPos);
  void     Save  (std::ofstream &ofs);
  void     Load  (std::ifstream &ifs);
  uint64_t ByteSize(){
    return b_.GetUsageBytes()
      + perm_.ByteSize()
      + sizeof(uint64_t);
  }
  void     Clear(){
    b_.Clear();
    perm_.Clear();
  }
private:
  int Compare(const uint64_t kIndex1,
	       const uint64_t kIndex2,
	       const uint64_t kVar1,
	       const uint64_t kVar2);
  void QSort(FFLCArray &fflca,
	     FFLCArray &perm_arrray,
	     const uint64_t kStart,
	     const uint64_t kEnd);
  void SetMaxVar(FFLCArray &fflca);
  void BuildGMRPermutation(FFLCArray &fflca,
			   const uint64_t kSkipInterval);
  void BuildGMRBitArray(FFLCArray &fflca);

private:
  rsdic::RSDic b_;
  Permutation perm_;
  uint64_t max_var_;
};

#endif //GMR_HPP
