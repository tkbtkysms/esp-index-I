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

#include <cmath>
#include <vector>

inline bool IsRepetition(const uint64_t kVar1, const uint64_t kVar2){
  return (kVar1 == kVar2);
}

inline uint64_t ComputeLCA(const uint64_t kCode1,const uint64_t kCode2){
  uint64_t v1,v2;
  uint64_t lca;  
  
  v1 = 2*kCode1 -1;
  v2 = 2*kCode2 -1;
  lca = v1 ^ v2;
  lca = (uint64_t)floor(log2(lca));
  
  return lca;
}

inline bool IsMinimal(const uint64_t kCode1, 
		      const uint64_t kCode2, 
		      const uint64_t kCode3){
  if((kCode1 > kCode2 ) && (kCode2 < kCode3)){
    return true;
  }
  else{
    return false;
  }
}

inline bool IsMaximal(std::vector<uint64_t> &array,
		      const uint64_t kPos,
		      const uint64_t kPrevious){
  
  if(ComputeLCA(array[kPos], array[kPos + 1]) > ComputeLCA(kPrevious, array[kPos]) 
     && ComputeLCA(array[kPos], array[kPos + 1]) > ComputeLCA(array[kPos + 1], array[kPos + 2])){
    return true;
  }
  else{
    return false;
  }  
}

inline bool IsPair(std::vector<uint64_t> &array,
		   const uint64_t kPos,
		   const uint64_t kPrevious){		   
      
  if(IsRepetition(array[kPos], array[kPos + 1])){  
    if(!IsRepetition(array[kPos + 1], array[kPos + 2]) 
       && !IsRepetition(array[kPos + 2], array[kPos + 3]) 
       && IsRepetition(array[kPos + 3], array[kPos + 4])){
      return false;
    }
    if(IsRepetition(array[kPos + 1], array[kPos + 2])){
      if(!IsRepetition(array[kPos + 2], array[kPos + 3])
	 && !IsRepetition(array[kPos + 3], array[kPos + 4]) 
	 && IsRepetition(array[kPos + 4], array[kPos + 5])){
	return true;
      }
      else if(IsRepetition(array[kPos + 2], array[kPos + 3])){
	return true;
      }
      return false;
    }
    return true;
  }
  if(IsRepetition(array[kPos + 2], array[kPos + 3])){
    return true;
  }
  if(IsRepetition(array[kPos + 3], array[kPos + 4])){
    return false;
  }
  if(IsRepetition(array[kPos + 4], array[kPos + 5])){
    return true;
  }
  if(IsMinimal(kPrevious, array[kPos], array[kPos + 1]) 
     || IsMaximal(array, kPos, kPrevious)){
    return true;
  }
  if(IsMinimal(array[kPos], array[kPos + 1], array[kPos + 2]) 
     || IsMaximal(array, kPos + 1, array[kPos])){
    return false;
  }
  
  return true;
}

#endif //IS_PAIR
