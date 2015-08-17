/* reverse_phrase_dictionary.cpp
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

#include "reverse_phrase_dictionary.hpp"

void RPDict::Init(const size_t kReserveLength){    

  for(size_t i = primes_index_ ; i < PRIMESIZE; i++){
    if(PRIMES[i] >= kReserveLength){
      primes_index_ = i;
      hash_size_ = PRIMES[primes_index_];
      break;
    }
    else if(i == PRIMESIZE - 1){
      primes_index_ = i;
      hash_size_ = PRIMES[i];
    }
  }
    
  hash_table_.resize(hash_size_);
  hash_list_.resize(hash_size_);
    
  for(size_t i = 0; i < hash_size_;i++){
    hash_table_[i] = DUMMYCODE;
    hash_list_[i] = DUMMYCODE;
  }

}
  
uint64_t RPDict::GetHashSize(){
  return hash_size_;
}

uint64_t RPDict::ReverseAccessToPRule(PDict &pdict,
				      const uint64_t kLeft, 
				      const uint64_t kRight){
  uint64_t hash_val = HashVal(GetHashSize(),
			      kLeft,
			      kRight);
  uint64_t key1,key2;
  if((key1 = GetValue(hash_val,true)) != DUMMYCODE){
    if(pdict.IsEqualPRule(pdict.LevelVarToVar(key1),
			  kLeft,
			  kRight)){
      return pdict.LevelVarToVar(key1);
    }
  }
  else{
    SetValue(hash_val,
	     pdict.VarToLevelVar(pdict.num_prules()),
	     true,
	     pdict);
    return pdict.num_prules();
  }
  while((key2 = GetValue(key1, false)) != DUMMYCODE){
    if(pdict.IsEqualPRule(pdict.LevelVarToVar(key2), 
			  kLeft, 
			  kRight)){
      return pdict.LevelVarToVar(key2);
    }
    key1 = key2;
  }
  SetValue(key1,
	   pdict.VarToLevelVar(pdict.num_prules()),
	   false,
	   pdict);

  return pdict.num_prules();
}
  
void RPDict::SetValue(const uint64_t kKey,
		     const uint64_t kValue,
		     const bool kIsTable,
		     PDict &pdict){
  
  if((kValue < hash_size_) || (kValue == DUMMYCODE)){
    if(kIsTable){
      hash_table_[kKey] = kValue;
    }
    else{
      hash_list_[kKey] = kValue;
    }
  }
  else{
    ReHash(pdict);
  }
}
  
uint64_t RPDict::GetValue(const uint64_t kKey,
			  const bool kIsTable){
  if(kIsTable){
    return hash_table_[kKey];
  }
  else{
    return hash_list_[kKey];
  }
}
  
void RPDict::ReHash(PDict &pdict){
  Resize();  
  for(size_t i = pdict.offset(pdict.offset_length() - 1); i < pdict.num_prules(); i++){
    uint64_t hash_val = HashVal(hash_size_,
				pdict.prules(i).left_,
				pdict.prules(i).right_); 
    uint64_t key1, key2;
    if((key1 = GetValue(hash_val,true)) == DUMMYCODE){
      SetValue(hash_val,
	       pdict.VarToLevelVar(i),
	       true,
	       pdict);
    }
    else{
      while((key2 = GetValue(key1,false)) != DUMMYCODE){
	key1 = key2;
      }
      SetValue(key1,
	       pdict.VarToLevelVar(i),
	       false,
	       pdict);
    }
  }
}
 
void RPDict::Resize(){    

  primes_index_++;
  hash_size_ = PRIMES[primes_index_];
    
  hash_table_.resize(hash_size_);
  hash_list_.resize(hash_size_);
    
  for(size_t i = 0; i < hash_size_;i++){
    hash_table_[i] = DUMMYCODE;
    hash_list_[i] = DUMMYCODE;;
  }

}

uint64_t RPDict::HashVal(const uint64_t kHashSize,
			const uint64_t kLeft,
			const uint64_t kRight){
  return ((kLeft + 5)* kRight * 521) % kHashSize;
}  


void RPDict::Clear(PDict &pdict){
  for(size_t i = 0;i < GetHashSize(); i++){
    SetValue(i,
	     DUMMYCODE,
	     true,
	     pdict);
    SetValue(i,
	     DUMMYCODE,
	     false,
	     pdict);
  }
}

void RPDict::Delete(){
  hash_size_ = 0;
  primes_index_ = 0;
  std::vector<uint64_t> ().swap(hash_table_);
  std::vector<uint64_t> ().swap(hash_list_);
}

uint64_t RPDict:: ByteSize(){
  return 2 * sizeof(uint64_t) + 2 * hash_size_ * sizeof(uint64_t);
}
