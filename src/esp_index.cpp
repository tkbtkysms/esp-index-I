/* esp_index.cpp
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

#include "esp_index.hpp"

using namespace std;
using namespace rsdic;

void ESPIndex::Build(ESPTree &esp_tree, 
		     const uint32_t kPositionRatio,
		     ofstream &ofs){
  cout << "Encode ESP tree" << endl;
  
  double encode_time = Gettime::get_time();
  
  SetESPTreeInfo(esp_tree);  
 
  BuildExtractionLength(esp_tree, kPositionRatio);

  BuildLeft(esp_tree);

  BuildRight(esp_tree);
  
  encode_time = Gettime::get_time() - encode_time;
  cout << "Finish encoding ESP tree" << endl;
  cout << "Encoding ESP tree time: " << encode_time << " sec" << endl;
  
  Save(ofs);
  ofs.close();
  Clear();
}

void ESPIndex::SetOffset(ESPTree &esp_tree){
  offset_.Init(level_, esp_tree.GetOffset(level_ - 1));
  for(size_t i = 0; i < level_; i++){
    offset_.Set(i, esp_tree.GetOffset(i));
  }
}

void ESPIndex::SetESPTreeInfo(ESPTree &esp_tree){
  num_prules_ = esp_tree.GetNumPRules();
  level_ = esp_tree.Level();
  root_ = esp_tree.root();
  char_map_ = esp_tree.char_map();
  SetOffset(esp_tree);
}

void ESPIndex::BuildExtractionLength(ESPTree &esp_tree, const uint32_t kPositionRatio){
 
  size_t length_index = 0;
  size_t sparse_size = 0;
  uint64_t previous_length = offset(0);
  uint64_t now_length = 0;

  RSDicBuilder bvb;
  for(size_t i = 1; i < level_; i++){
    now_length = offset(i);
    if((i % kPositionRatio) == 0){
      sparse_size += now_length - previous_length;
      for(size_t j = previous_length; j < now_length; j++){
	bvb.PushBack(1);
	length_index++;
      }
    }
    else{
      for(size_t j = previous_length; j < now_length; j++){
	bvb.PushBack(0);
      }
    }
    previous_length = now_length;
  }
  extraction_length_.Init(length_index, esp_tree.extraction_length(root_));
  previous_length = offset(0);
  now_length = 0;
  length_index = 0;
  for(size_t i = 0; i < level_; i++){
    now_length = offset(i);
    if(i % kPositionRatio == 0){
      for(size_t j = previous_length; j < now_length; j++){
	extraction_length_.Set(length_index++, esp_tree.extraction_length(j));
      }
    }
    previous_length = now_length;
  }

  bvb.Build(sparse_bit_);

}

void ESPIndex::BuildLeft(ESPTree &esp_tree){

  uint64_t previous_code = 0;
  uint64_t num_zero = 0;
  RSDicBuilder bvb;

  for(size_t i = CHARSIZE; i < num_prules();i++){
    num_zero = esp_tree.GetPRule(i).left_ - previous_code;
    for(size_t j = 0; j < num_zero; j++){
      bvb.PushBack(0);
    }
    bvb.PushBack(1);
    previous_code = esp_tree.GetPRule(i).left_;
  }

  bvb.Build(left_);
}

void ESPIndex::BuildRight(ESPTree &esp_tree){
  FFLCArray gmr_seq;
  uint64_t perm_skip_interval = floor(log2(floor(log2(num_prules())) + 1)) + 1;
  gmr_seq.Init(num_prules(), num_prules());
  for(size_t i = CHARSIZE; i < num_prules(); i++){
    gmr_seq.Set(i - CHARSIZE, esp_tree.GetPRule(i).right_);
  }

  if(perm_skip_interval <= 0 || perm_skip_interval >= num_prules()){
    perm_skip_interval = 1;
  }
  
  right_.Build(gmr_seq, perm_skip_interval);
}


uint64_t ESPIndex::left(const uint64_t kVar){
  return left_.Select(kVar - CHARSIZE, 1) - (kVar - CHARSIZE); 
}

uint64_t ESPIndex::right(const uint64_t kVar){
  return right_.Access(kVar - CHARSIZE);
 
}

uint64_t ESPIndex::FirstLeftParentBitPos(const uint64_t kVar){
  if(kVar > left(num_prules() - 1)){
    return DUMMYCODE;
  }

  if(kVar == 0){
    if(left_.GetBit(0) == 1){
      return 0;
    }
    else{
      return DUMMYCODE;
    }
  }else{
    return left_.Select(kVar - 1, 0) + 1;
  }
}

uint64_t ESPIndex::LeftParent(const uint64_t kVar,
			      const uint64_t kFirstParentLeftBitPos,
			      const uint64_t kParentNum){
  
  if(left_.GetBit(kFirstParentLeftBitPos + kParentNum) == 1){
    return  kFirstParentLeftBitPos + kParentNum - kVar + CHARSIZE; 
  }
  else{
    return DUMMYCODE;
  }

}

uint64_t ESPIndex::RightParent(const uint64_t kVar,const uint64_t kParentNum){
  uint64_t select = right_.Select(kVar, kParentNum + 1);
  if(select == DUMMYCODE){
    return DUMMYCODE;
  }
  return select + CHARSIZE;
}

uint64_t ESPIndex::ReverseAccessToPRule(const uint64_t kLeft,const uint64_t kRight){

  if(kLeft >= num_prules() || kRight >= num_prules() ){
    return DUMMYCODE;
  }
  uint64_t rank1;
  uint64_t left_hand_side;
  if(kLeft != 0){
    rank1 = left_.Select(kLeft - 1, 0) - kLeft + 1;
    left_hand_side = right_.Select(kRight, right_.Rank(kRight, rank1) + 1);
  }
  else{
    left_hand_side = right_.Select(kRight, 1);
  }
  if(left_hand_side <= (left_.Select(kLeft, 0) - kLeft)){
    return left_hand_side + CHARSIZE;
  }
  else{
    return DUMMYCODE;
  }
}


uint64_t ESPIndex::num_prules(){
  return num_prules_;
}

uint64_t ESPIndex::root(){
  return root_;
}

char ESPIndex::GetVarToChar(const uint64_t kVariable){
  return char_map_.var_to_char(kVariable);
}

uint64_t ESPIndex::GetCharToVar(const char kCharacter){
  return char_map_.char_to_var(kCharacter);
}

uint64_t ESPIndex::offset(const size_t kHeight){
  return offset_[kHeight];
}

uint64_t ESPIndex::extraction_length(const size_t kVar){
  if(kVar < CHARSIZE){
    return 1;
  }
  else if(!sparse_bit_.GetBit(kVar - CHARSIZE)){
    return extraction_length(left(kVar)) 
      + extraction_length(right(kVar));
  }
  else{
    return extraction_length_[sparse_bit_.Rank(kVar - CHARSIZE, 1)];
  }

}

void ESPIndex::RepeatDecompress(const uint64_t kIndex, 
				ofstream &ofs,
				vector<uint64_t>&tmp_left,
				vector<uint64_t> &tmp_right){
  if(kIndex < CHARSIZE){
    ofs << char_map_.var_to_char(kIndex);
  }
  else{
    if(tmp_left[kIndex] == DUMMYCODE){
      tmp_left[kIndex] = left(kIndex);
    }
    RepeatDecompress(tmp_left[kIndex],
		     ofs,
		     tmp_left,
		     tmp_right);
    if(tmp_right[kIndex] == DUMMYCODE){
      tmp_right[kIndex] = right(kIndex);
    }
    RepeatDecompress(tmp_right[kIndex],
		     ofs,
		     tmp_left,
		     tmp_right);
  }
}

void ESPIndex::Decompress(ifstream &ifs,ofstream &ofs){
  Load(ifs);
  ifs.close();

  vector<uint64_t> tmp_left;
  vector<uint64_t> tmp_right;
  tmp_left.resize(num_prules());
  tmp_right.resize(num_prules());
  for(uint64_t i = 0; i < num_prules();i++){
    tmp_left[i] = DUMMYCODE;
    tmp_right[i] = DUMMYCODE;
  }
  RepeatDecompress(root(), 
	 ofs,
	 tmp_left,
	 tmp_right);
  ofs.close();
}

void ESPIndex::Save(ofstream &ofs){
  
  ofs.write((char *)&level_, sizeof(uint64_t));
  offset_.Save(ofs);
  ofs.write((char *)&num_prules_, sizeof(uint64_t));
  sparse_bit_.Save(ofs);
  extraction_length_.Save(ofs);
  ofs.write((char *)&root_,sizeof(uint64_t));
  char_map_.Save(ofs);
  left_.Save(ofs);
  right_.Save(ofs);

  cout << "Index size: " << right_.ByteSize() 
    + left_.GetUsageBytes() 
    + (3 + CHARSIZE) * sizeof(uint64_t) 
    + CHARSIZE * sizeof(char) 
    + offset_.ByteSize() << " byte" << endl;
   
  cout <<  "extraction length size: " 
       << sparse_bit_.GetUsageBytes() 
    + extraction_length_.ByteSize() 
       << " byte" << endl;
   
  cout << "Total size: " 
       << sparse_bit_.GetUsageBytes() 
    + extraction_length_.ByteSize()
    + offset_.ByteSize()
    + right_.ByteSize() 
    + left_.GetUsageBytes() 
    + (3 + CHARSIZE) * sizeof(uint64_t) 
    + CHARSIZE * sizeof(char)
       << " byte" << endl;

}


void ESPIndex::Load(ifstream &ifs){
  
  ifs.read((char *)&level_, sizeof(uint64_t));
  offset_.Load(ifs);
  ifs.read((char *)&num_prules_, sizeof(uint64_t));
  sparse_bit_.Load(ifs);
  extraction_length_.Load(ifs);
  ifs.read((char *)&root_, sizeof(uint64_t));
  char_map_.Load(ifs);
  left_.Load(ifs);
  right_.Load(ifs);
  
}
