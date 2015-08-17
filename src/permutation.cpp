/* permutation.cpp
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
 
#include "permutation.hpp"

using namespace std;
using namespace rsdic;

void Permutation::Build(FFLCArray elems, const uint64_t skip_length){
  
  elems_  = move(elems);  

  FFLCArray tmp_skip_list;
  tmp_skip_list.Init(elems_.length(), elems_.length());
  for(size_t i = 0; i < elems_.length(); i++){
    tmp_skip_list.Set(i, elems_.length());
  }
  
  vector<bool> is_check_elem;
  is_check_elem.resize(elems_.length());
  vector<bool> (is_check_elem).swap(is_check_elem);

  for(size_t i = 0;i < elems_.length(); i++){
    is_check_elem[i] = false;
  }

  uint64_t skip_list_num = 0;

  for(size_t i = 0; i < elems_.length(); i++){
    if(!is_check_elem[i]){
      uint64_t pos = i;
      uint64_t loop_length = 0;
      uint64_t skip_pos = i;
      uint64_t local_skip_num = 0;
      
      while(elems_[pos] != i){
	is_check_elem[pos] = true; 
	loop_length++;
	if(loop_length == skip_length){
	  loop_length = 0;
	  skip_list_num++;
	  local_skip_num++;
	  tmp_skip_list.Set(pos, skip_pos);
	  skip_pos = pos;
	}
	pos = elems_[pos];
      }
      is_check_elem[pos] = true; 
      if(local_skip_num > 0){
	skip_list_num++;
	tmp_skip_list.Set(i, skip_pos);
      }
    }    
  }
  
  skip_list_.Init(skip_list_num, elems_.length() - 1);

  RSDicBuilder bvb;
  uint64_t skip_list_index = 0;
  for(size_t i = 0; i < elems_.length(); i++){
    if(tmp_skip_list[i] !=  elems_.length()){
      bvb.PushBack(1);
      skip_list_.Set(skip_list_index++, tmp_skip_list[i]);      
    }
    else{
      bvb.PushBack(0);
    }
  }
  bvb.Build(has_skip_elem_);
}

uint64_t Permutation::Access(const uint64_t kPos){
  return elems_[kPos];
}

uint64_t Permutation::ReverseAccess(const uint64_t kPos){

  uint64_t pos = kPos;

  while(elems_[pos] != kPos){
    if(has_skip_elem_.GetBit(pos) ){
      pos = skip_list_[has_skip_elem_.Rank(pos + 1, 1) - 1];
      while(elems_[pos] != kPos){
	pos = elems_[pos];
      }
      break;
    }
    else{
      pos = elems_[pos];
    }
  }

  return pos;
}

uint64_t Permutation::Length(){
  return elems_.length();
}

void Permutation::Clear(){
  elems_.Clear();
  has_skip_elem_.Clear();
  skip_list_.Clear();
}

void Permutation::Save(ofstream &ofs){
  elems_.Save(ofs);
  has_skip_elem_.Save(ofs);
  skip_list_.Save(ofs);
}

void Permutation::Load(ifstream &ifs){
  elems_.Load(ifs);
  has_skip_elem_.Load(ifs);
  skip_list_.Load(ifs);
}
