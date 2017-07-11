/* is_pair.hpp
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

#ifndef ISPAIR_HPP
#define ISPAIR_HPP
#ifdef __SSE4_2__
#include <xmmintrin.h>
#endif


#include <cmath>
#include <vector>

inline bool IsRepetition(std::vector<uint64_t> &array,
			 const uint64_t kPos){
  return (array[kPos] == array[kPos + 1]);
}


static const uint64_t kdeBruijnNum64 = 0x03F566ED27179461UL;

static const uint8_t kLSB_TBL[64] =
{ 0, 1,59, 2,60,40,54, 3,
  61,32,49,41,55,19,35, 4,
  62,52,30,33,50,12,14,42,
  56,16,27,20,36,23,44, 5,
  63,58,39,53,31,48,18,34,
  51,29,11,13,15,26,22,43,
  57,38,47,17,28,10,25,21,
  37,46, 9,24,45, 8, 7, 6
};

static const uint64_t kOne = 1ULL;

inline static uint32_t LSB(uint64_t bits){
  if(bits == 0){
    return 0;
  }
#ifdef __SSE4_2__
  return __builtin_ctzll(bits);
#else
  return kLSB_TBL[((bits& -bits)*kdeBruijnNum64) >> 58];
#endif
}

inline uint64_t AlphabetReduction(const uint64_t kFirst,
				   const uint64_t kSecond){

  uint64_t diff_bit_pos = LSB(kFirst ^ kSecond);

  if((kSecond >> diff_bit_pos) & kOne){
    return (diff_bit_pos << 1) + 1;
  }
  else{
    return (diff_bit_pos << 1);
  }
}


inline bool IsMinimal(std::vector<uint64_t> &array,
		      const uint64_t kPos){
  if(kPos > 2
     && !IsRepetition(array, kPos - 2)
     && !IsRepetition(array, kPos - 1)){
    uint64_t middle = AlphabetReduction(array[kPos - 1], array[kPos]);
    if((middle < AlphabetReduction(array[kPos - 2], array[kPos -1])) 
       && (middle < AlphabetReduction(array[kPos], array[kPos + 1]))){
      return true;
    }
  }
  return false;
}

inline bool IsMaximal(std::vector<uint64_t> &array,
		      const uint64_t kPos){
  if(kPos > 2
     && !IsRepetition(array, kPos - 2)
     && !IsRepetition(array, kPos - 1)){
    uint64_t middle = AlphabetReduction(array[kPos - 1], array[kPos]);
    if((middle > AlphabetReduction(array[kPos - 2], array[kPos - 1]))
       &&( middle > AlphabetReduction(array[kPos], array[kPos + 1]))){
      return true;
    }
  }
  return false;
}

inline bool Is2Tree(std::vector<uint64_t> &array,
		    const uint64_t kPos){		   
      
  if(IsRepetition(array, kPos)){//aa....
    if(IsRepetition(array, kPos + 1)){//aaa...
      if(!IsRepetition(array, kPos + 2)
	 && !IsRepetition(array, kPos + 3) 
	 && IsRepetition(array, kPos + 4)){//aaabcc
	return true;
      }
      else if(IsRepetition(array, kPos + 2)){//aaaa.. 
	return true;
      }
      return false;  // other aaa...
    }
    else if(!IsRepetition(array, kPos + 2)
	    && IsRepetition(array, kPos + 3)){//aacbb.
      return false;
    }
    else{//other aa.... 
      return true;
    }
  }
  if(IsRepetition(array, kPos + 1)){//baa..
    if(IsRepetition(array, kPos + 2)){
      if(!IsRepetition(array, kPos + 3)){//baaacd 
	return true;
      }
    }
    else if(!IsRepetition(array, kPos + 3)
	    && IsRepetition(array, kPos + 4)){//baacdd
      return true;
    }
    else{
      return false;
    }
  }
  if(IsRepetition(array, kPos + 2)){//bcaa..
    return true;
  }
  if(IsRepetition(array, kPos + 3)){//bcdaa.
    return false;
  }
  if(IsRepetition(array, kPos + 4)){//bcdeaa
    return true;
  }
  if(IsMaximal(array, kPos + 1)){
    return false;
  }
  if(IsMaximal(array, kPos)){
    return true;
  }
  if(IsMaximal(array, kPos + 2)){
    return true;
  }
  if(IsMinimal(array, kPos + 1)){
    return false;
  }
  return true;
}

#endif //IS_PAIR
