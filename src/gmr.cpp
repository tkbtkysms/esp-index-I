/* gmr.cpp
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

#include "gmr.hpp"

using namespace std;
using namespace rsdic;

int GMR::Compare(const uint64_t kIndex1,
		 const uint64_t kIndex2,
		 const uint64_t kVar1,
		 const uint64_t kVar2){

  if(kVar1 < kVar2){
    return 1;
  }
  else if(kVar1 > kVar2){
    return -1;
  }
  else{
    if(kIndex1 < kIndex2){
      return 1;
    }
    else if(kIndex1 > kIndex2){
      return -1;
    }
    else{
      return 0;
    }
  }
}

void GMR::QSort(FFLCArray &fflca,
		FFLCArray &perm_array,
		const uint64_t kStart,
		const uint64_t kEnd){

  uint64_t pivot_index = perm_array[(kEnd - kStart)/2 + kStart];
  uint64_t pivot = fflca[pivot_index];
  uint64_t i = kStart;
  uint64_t j = kEnd;
  
  while(true){
    while(i <= kEnd && Compare(perm_array[i],
			       pivot_index,
			       fflca[perm_array[i]],
			       pivot) == 1){
      i++;
    }
    while(j >= kStart && Compare(perm_array[j],
				 pivot_index,
				 fflca[perm_array[j]],
				 pivot) == -1){
      j--;
    }
    if(i >= j)
      break;
    perm_array.Swap(i, j);
    i++;
    j--;
  }

  if(i != 0 && (kStart < (i - 1)))QSort(fflca,
					perm_array,
				        kStart,
					i - 1);
  
  if((j + 1) < kEnd)QSort(fflca,
			  perm_array,
			  j + 1,
			  kEnd);
}

void GMR::SetMaxVar(FFLCArray &fflca){
  max_var_ = 0;
  for(size_t i = 0; i < fflca.length(); i++){
    if(max_var_ < fflca[i]){
      max_var_ = fflca[i];
    }
  }
}

void GMR::BuildGMRPermutation(FFLCArray &fflca, const uint64_t kSkipInterval){
 
  FFLCArray perm_array;
  perm_array.Init(fflca.length(),fflca.length() - 1);

  for(size_t i = 0; i < perm_array.length(); i++){
    perm_array.Set(i, i);
  }
  
  QSort(fflca, 
	perm_array,
	0, 
	fflca.length() - 1);
 
  perm_.Build(perm_array, kSkipInterval);

}

void GMR::BuildGMRBitArray(FFLCArray &fflca){

  uint64_t previous = fflca[perm_.Access(0)];
  RSDicBuilder bvb;
  bvb.Clear();
  previous = 0;
  for(size_t i = 0; i < perm_.Length();i++){
    for(size_t j = previous; j < fflca[perm_.Access(i)]; j++){
      bvb.PushBack(false);
    }
    bvb.PushBack(true);    
    previous = fflca[perm_.Access(i)];
  }
  bvb.PushBack(false);
  bvb.Build(b_);
}

void GMR::Build(FFLCArray &fflca, const uint64_t kPermSkipInterval){

  SetMaxVar(fflca);
  
  BuildGMRPermutation(fflca, kPermSkipInterval);

  BuildGMRBitArray(fflca);
 
}

uint64_t GMR::Access(const uint64_t kPos){
  if(kPos < perm_.Length()){
    uint64_t reverse_index = perm_.ReverseAccess(kPos);
    return b_.Select(reverse_index, 1) - reverse_index; 
  }
  else{
    return DUMMYCODE;
  }
}

uint64_t GMR::Rank(const uint64_t kVar, const uint64_t kPos){
  
  if((kVar > max_var_)|| (kPos > perm_.Length() + 1) ){
    return 0;
  }
  else{
    uint64_t select1 = 0;
    if(kVar != 0){
      select1 = b_.Select(kVar - 1, 0) + 1;
    }
    uint64_t select2 = b_.Select(kVar, 0);
    if(select1 != select2){
      uint64_t start = select1 - kVar;
      uint64_t end = select2 - (kVar + 1);
      uint64_t i = start;
      uint64_t j = end;
      uint64_t num = (j - i)/2 + start;
    
      while(i < j){
	if(perm_.Access(num) < kPos){ 
	  if(kPos <= perm_.Access(num + 1)){
	    break;
	  }
	  else{
	    if(num == j){
	      i = num;
	    }
	    else{
	      i = num + 1;
	    }
	  }
	}
	else{
	  if(num == i){
	    j = num;
	  }
	  else{
	    j = num - 1;
	  }
	}
	num = (j - i)/2 + i;
      }
      if(perm_.Access(num) < kPos){
	return num - start + 1;
      }
      else{
	return num - start;
      }
    }
    else{
      return 0;
    }
  }
}

uint64_t GMR::Select(const uint64_t kVar, const uint64_t kPos){
  if((kVar == 0) && (kPos > 0)){
    if(b_.GetBit(0) == 0){
      return DUMMYCODE;
    }    
    uint64_t select2 = b_.Select(kVar, 0);
    if((kPos - 1) < select2){
      return perm_.Access(kPos - 1);
    }
  }
  else if((kVar <= max_var_) && (kPos > 0)){
    uint64_t select1 = b_.Select(kVar - 1 , 0);
    uint64_t select2 = b_.Select(kVar, 0);
    if(kPos < (select2 - select1)){
      return perm_.Access(select1 - kVar + kPos);
    }
  }
  return DUMMYCODE;
}

void GMR::Save(ofstream &ofs){
  b_.Save(ofs);
  perm_.Save(ofs);
  ofs.write((char*)&max_var_, sizeof(uint64_t));
}

void GMR::Load(ifstream &ifs){
  b_.Load(ifs);
  perm_.Load(ifs);
  ifs.read((char*)&max_var_, sizeof(uint64_t));
}
