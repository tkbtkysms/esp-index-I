/* phrase_dictionary.cpp
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

#include "phrase_dictionary.hpp"

using namespace std;

void PDict::Init(const uint64_t kLen){
  num_prules_ = 0;
  prules_.resize(kLen);
  std::vector<PRule> (prules_).swap(prules_);
  prules_.clear();
  offset_length_ = 0;
  offset_.clear();
  SetOffset();
}

uint64_t PDict::num_prules(){
  return num_prules_ + CHARSIZE;
} 

void PDict::PushPRule(const uint64_t kLeft, 
		      const uint64_t kRight){
  num_prules_++;
  prules_.push_back(PRule(kLeft,
			  kRight));
}

void PDict::SetPRule(const uint64_t kVar,
		     const PRule kPRule){
  prules_[kVar - CHARSIZE].left_ = kPRule.left_;
  prules_[kVar - CHARSIZE].right_ = kPRule.right_;
}
  
PRule PDict::prules(const uint64_t kVar){
  return prules_[kVar - CHARSIZE];
}

bool PDict::IsEqualPRule(const size_t kVar, 
			 const size_t kLeft, 
			 const size_t kRight){
  return (prules(kVar).left_ == kLeft) && (prules(kVar).right_ == kRight);
}


Permutation *PDict::Sort(){
  uint64_t array_size = num_prules() - offset(offset_length() - 1);
  FFLCArray index;
  index.Init(array_size, array_size);
  for(size_t i = 0; i < array_size;i++){
    index.Set(i, i); 
  }

  QSort(index,
	0,
	array_size - 1);
    
  Permutation *perm_index = new Permutation();    
  perm_index->Build(index, 1);
  std::vector<PRule> tmp_prules;
  uint64_t right;

  for(size_t i = 0; i < array_size;i++){
    tmp_prules.push_back(prules(perm_index->Access(i) + offset(offset_length() - 1)));
    if((right = tmp_prules[i].right_) >= offset(offset_length() - 1)){
      tmp_prules[i].right_ = perm_index->ReverseAccess(right - offset(offset_length() - 1)) + offset(offset_length() - 1);
    }
  }

  for(size_t i = 0; i < array_size ; i++){
    SetPRule(i + offset(offset_length() - 1), tmp_prules[i]);
  }
    
  return perm_index;
}
  
void PDict::QSort(FFLCArray &array,
		  const uint64_t kStart,
		  const uint64_t kEnd){
    
  uint64_t pivot = array[(kEnd - kStart)/2 + kStart] + offset(offset_length() - 1);
  uint64_t i = kStart;
  uint64_t j = kEnd;

  while(true){
    while(i <= kEnd && (ComparePRule(array[i] + offset(offset_length() - 1), pivot) == 1)){
      i++;
    }
    while(j >= kStart && (ComparePRule(array[j] + offset(offset_length() - 1), pivot) == -1)){
      j--;
    }
    if(i >= j){
      break;
    }
    array.Swap(i, j);
    i++;
    j--;
  }
  if(i != 0 && kStart < i - 1)QSort(array,
				    kStart,
				    i - 1);
  if(j + 1 < kEnd)QSort(array,
			j + 1,
			kEnd);
}
  
int PDict::ComparePRule(const uint64_t kVar1,
			const uint64_t kVar2){
  
  PRule prule1 = prules(kVar1);
  PRule prule2 = prules(kVar2);
  if(prule1.left_ > prule2.left_){
    return -1;
  }
  else if(prule1.left_ < prule2.left_){
    return 1;
  }
  else{
    if((prule1.right_  >= offset(offset_length() - 1)) 
       && (prule2.right_ >= offset(offset_length() - 1))){
      return ComparePRule(prule1.right_, prule2.right_);
    }
    else if(prule1.right_ > prule2.right_){
      return -1;
    }
    else if(prule1.right_ < prule2.right_){
      return 1;
    }else{
      return 0;
    }

  }
}

void PDict::SortCheck(){
  uint64_t previous_left = prules(CHARSIZE).left_;
  uint64_t previous_right = prules(CHARSIZE).right_;

  for(size_t i = CHARSIZE + 1; i < num_prules(); i++){
    if(previous_left > prules(i).left_){
      std::cout << i - 1 << " " << prules(i - 1).left_ << " " << prules(i - 1).right_ << std::endl;
      std::cout << i << " " << prules(i).left_ << " " << prules(i).right_ << std::endl;
    }
    if(previous_left == prules(i).left_ && previous_right ==prules(i).right_){
      std::cout << i - 1 << " " << prules(i - 1).left_ << " " << prules(i - 1).right_ << std::endl;
      std::cout << i << " " << prules(i).left_ << " " << prules(i).right_ << std::endl;
    }
    previous_left = prules(i).left_;
    previous_right = prules(i).right_;
  }
}

void PDict::SetOffset(){
  offset_.push_back(num_prules());
  offset_length_++;
}

uint64_t PDict::offset_length(){
  return offset_length_;
}

uint64_t PDict::offset(const uint64_t kLevel){
  return offset_[kLevel];
}

uint64_t PDict::VarToLevelVar(const uint64_t kVar){
  return kVar - offset(offset_length() - 1);
}

uint64_t PDict::LevelVarToVar(const uint64_t kLevelVar){
  return kLevelVar + offset(offset_length() - 1);
}  

void PDict::AdjustLength(){
  prules_.resize(num_prules());
  vector<PRule> (prules_).swap(prules_);
  offset_.resize(offset_length());
  vector<uint64_t> (offset_).swap(offset_);
}
  
void PDict::Clear(){
  vector<PRule> ().swap(prules_);
  num_prules_ = 0;
  vector<uint64_t> ().swap(offset_);
  offset_length_ = 0;
}

uint64_t PDict::ByteSize(){ 
  return sizeof(uint64_t) 
    + 2 * num_prules() * sizeof(uint64_t)
    + offset_.size() * sizeof(uint64_t);
}
