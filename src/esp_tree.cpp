/* esp_tree.cpp
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

#include"esp_tree.hpp"

using namespace std;

void ESPTree::Init(const uint64_t kReserveLength){
  
  uint64_t reserve_length = kReserveLength + CHARSIZE;
  
  pdict_.Init(reserve_length);
  rpdict_.Init(reserve_length);
}

void ESPTree::Build(ifstream &ifs){
  double time = Gettime::get_time();

  input_buffer_pos_  = 0;
  previous_ = DUMMYCODE;
  next_level_len_ = 0;
  remaining_len_ = 0;
  
  Init(4 * TRANSMB / sizeof(uint64_t) + 1);  
  cout << "Read input text" << endl;
  ReadInputText(ifs, DUMMYCODE);
  cout << "Finish reading input text" << endl;
  
  cout << "Build ESP tree" << endl;
  
  while(remaining_len_ != 1){
    
    previous_ = DUMMYCODE;
    input_buffer_pos_ = 0;
    next_level_len_ = 0;
    
    while(remaining_len_ > 5){
      if(IsPair(input_buffer_, 
		input_buffer_pos_,
		previous_)){
	Build2Tree();
      }
      else{
	Build2_2Tree();
      }
    }
    if(remaining_len_ == 5){
      Build2Tree();
      Build2_2Tree();
    }
    else if(remaining_len_  == 4){
      Build2Tree();
      Build2Tree(); 
    }
    else if(remaining_len_ == 3){
      Build2_2Tree();
    }
    else if(remaining_len_  == 2){
      Build2Tree();
    }
    
    remaining_len_ = next_level_len_;

    Permutation *perm_index = pdict_.Sort();

    RenameExtractionLength(perm_index);

    RenameNextLevelVars(perm_index,
			0,
			1);

    perm_index->Clear();
    rpdict_.Clear(pdict_);
    pdict_.SetOffset();
  }
  SetRoot(input_buffer_[0]);  

  time = Gettime::get_time() - time;
  cout << "Finish building ESP tree" << endl;
  cout <<"Total production rules : " << pdict_.num_prules() << endl;
  cout << "Build ESP tree time: "<< time << " sec" << endl;
}

void ESPTree::DiskBuild(ifstream &ifs, const uint64_t kInputSpace){

  double time = Gettime::get_time();
  input_buffer_pos_  = 0;
  previous_ = DUMMYCODE;
  next_level_len_ = 0;
  remaining_len_ = 0;
  
  Init(4 * TRANSMB / sizeof(uint64_t) + 1);  
  cout << "Rename input characters to variables" << endl;
  ReadInputText(ifs, kInputSpace);
  cout << "Finish renaming input characters to variables" << endl;
  
  cout << "Build ESP tree" << endl;
  
  ifstream ifs_disk_buffer;
  ofstream ofs_disk_buffer;

  uint64_t read_length = 0;
  uint64_t total_read_length = 0;
  uint64_t total_next_level_len = 0;

  uint64_t input_length = remaining_len_;
  uint64_t num_remaining_vars = 0;
  
  while(input_length != 1){
    
    ifs_disk_buffer.open("DiskBuffer1");
    ofs_disk_buffer.open("DiskBuffer2");
    
    remaining_len_ = 0;
    num_remaining_vars = 0;
    previous_ = DUMMYCODE;
    read_length = kInputSpace * TRANSMB;
    total_read_length = 0;
    total_next_level_len = 0;
    
    if(read_length > input_length){
      read_length = input_length;
    }
    
    while(ifs_disk_buffer.read((char*)&input_buffer_[num_remaining_vars], (read_length - num_remaining_vars) * sizeof(uint64_t)) 
	  && total_read_length != input_length){
      input_buffer_pos_ = 0;
      next_level_len_ = 0;
      remaining_len_ = read_length;
      
      while(remaining_len_ > 5){
	if(IsPair(input_buffer_, 
		  input_buffer_pos_,
		  previous_)){
	  Build2Tree();
	}
	else{
	  Build2_2Tree();
	}
      }
      ofs_disk_buffer.write((char*)&input_buffer_[0], next_level_len_ * sizeof(uint64_t));
 
      total_next_level_len += next_level_len_;
    
      for(size_t i = 0; i < remaining_len_; i++){
	input_buffer_[i] = input_buffer_[input_buffer_pos_++];
      }
    
      total_read_length += read_length - num_remaining_vars;
      
      num_remaining_vars = remaining_len_;
      if((total_read_length + read_length - num_remaining_vars) > input_length){
	read_length = input_length - total_read_length + num_remaining_vars;
      }
    }
    
    ifs_disk_buffer.close();
    
    input_buffer_pos_ = 0;
    next_level_len_ = 0;
    
    if(remaining_len_ == 5){
      Build2Tree();
      Build2_2Tree();
    }
    else if(remaining_len_  == 4){
      Build2Tree();
      Build2Tree(); 
    }
    else if(remaining_len_ == 3){
      Build2_2Tree();
    }
    else if(remaining_len_  == 2){
      Build2Tree();
    }
    
    ofs_disk_buffer.write((char*)&input_buffer_[0], sizeof(uint64_t) * next_level_len_);
    ofs_disk_buffer.close();

    total_next_level_len += next_level_len_;
    
    input_length = total_next_level_len;
    Permutation *perm_index = pdict_.Sort();

    RenameExtractionLength(perm_index);
    
    RenameNextLevelVars(perm_index,
			kInputSpace,
			2);

    perm_index->Clear();
    rpdict_.Clear(pdict_);
    pdict_.SetOffset();
  }
  remove("DiskBuffer1");
  remove("DiskBuffer2");

  SetRoot(input_buffer_[0]);  

  time = Gettime::get_time() - time;
  cout << "Finish building ESP tree" << endl;
  cout <<"Total production rules : " << pdict_.num_prules() << endl;
  cout << "Build ESP tree time: "<< time << " sec" << endl;
}

void ESPTree::ReadInputText(ifstream &ifs, const uint64_t kLen){
  
  vector<char> char_buffer;
  uint64_t read_length;
  uint64_t total_read_length;
  uint64_t input_buffer_pos;
  uint64_t input_length;
  
  if(kLen == DUMMYCODE){
    read_length = 10 * TRANSMB;
    total_read_length = 0;
    input_buffer_pos = 0;
    char_buffer.resize(read_length);
  
    ifs.seekg(0,ios::end);
    input_length = ifs.tellg();
    ifs.seekg(0,ios::beg);
    
    remaining_len_ = input_length;
    
    input_buffer_.resize(input_length);
    vector<uint64_t> (input_buffer_).swap(input_buffer_);
  
    
    if(read_length > input_length){
      read_length = input_length;
    }
    
    while(ifs.read((char*)&char_buffer[0], read_length) 
	  && total_read_length != input_length){
      
      for(size_t i = 0; i < read_length; i++){
	input_buffer_[input_buffer_pos++] = char_map_.char_to_var(char_buffer[i]);
      }
      
      total_read_length += read_length;
    
      if((total_read_length + read_length) > input_length){
	read_length = input_length - total_read_length;
      }
   
    }
    char_buffer.clear();
    vector<char> (char_buffer).swap(char_buffer);
    ifs.close();
  
  } 
  else{
    
    ofstream disk_buffer("DiskBuffer1");
    read_length = kLen * TRANSMB;;
    total_read_length = 0;
    input_buffer_pos = 0;
    char_buffer.resize(read_length);
    input_buffer_.resize(read_length);
    vector<uint64_t> (input_buffer_).swap(input_buffer_);
    
    ifs.seekg(0,ios::end);
    input_length = ifs.tellg();
    ifs.seekg(0,ios::beg);
    
    remaining_len_ = input_length;
    
    if(read_length > input_length){
      read_length = input_length;
    }
    
    while(ifs.read((char*)&char_buffer[0], read_length) 
	  && total_read_length != input_length){
      input_buffer_pos = 0;
      for(size_t i = 0; i < read_length; i++){
	input_buffer_[input_buffer_pos++] = char_map_.char_to_var(char_buffer[i]);
      }
      
      disk_buffer.write((char*)&input_buffer_[0], read_length * sizeof(uint64_t) );
      total_read_length += read_length;
    
      if((total_read_length + read_length) > input_length){
	read_length = input_length - total_read_length;
      } 
    }
    disk_buffer.close();
    char_buffer.clear();
    vector<char> (char_buffer).swap(char_buffer);
    ifs.close();
  }
 
}

void ESPTree::Build2Tree(){
  uint64_t var,left, right;

  var = rpdict_.ReverseAccessToPRule(pdict_,
				     (left = input_buffer_[input_buffer_pos_]), 
				     (right  = input_buffer_[input_buffer_pos_ + 1]));
  if(var == pdict_.num_prules()){
    pdict_.PushPRule(left, right);
    extraction_length_.push_back(extraction_length(left) 
				 + extraction_length(right));
  }
  input_buffer_[next_level_len_++] = var;
  previous_ = right;
  input_buffer_pos_ += 2;
  remaining_len_ -= 2;
}

void ESPTree::Build2_2Tree(){
  uint64_t var1, var2, left, right;
  var1 = rpdict_.ReverseAccessToPRule(pdict_,
				      (left = input_buffer_[input_buffer_pos_ + 1]), 
				      (right  = input_buffer_[input_buffer_pos_ + 2]));  
  if(var1 == pdict_.num_prules()){
    pdict_.PushPRule(left, right);
    extraction_length_.push_back(extraction_length(left) 
				 + extraction_length(right));
  }
  var2 = rpdict_.ReverseAccessToPRule(pdict_,
				      (left = input_buffer_[input_buffer_pos_]), 
				      var1);
  if(var2 == pdict_.num_prules()){
    pdict_.PushPRule(left, var1);
    extraction_length_.push_back(extraction_length(left) 
				 + extraction_length(var1));
  }
  input_buffer_[next_level_len_++] =  var2;
  previous_ = right;
  input_buffer_pos_ += 3;
  remaining_len_ -= 3;
}

void ESPTree::RenameExtractionLength(Permutation *perm_index){
  
  vector<uint64_t> tmp_extraction_length;
  
  tmp_extraction_length.resize(pdict_.num_prules() - GetOffset(Level() - 1));
  vector<uint64_t> (tmp_extraction_length).swap(tmp_extraction_length);
  tmp_extraction_length.clear();
  
  for(size_t i = GetOffset(Level() - 1) ; i < pdict_.num_prules(); i++){
    tmp_extraction_length.push_back(extraction_length(pdict_.LevelVarToVar(perm_index->Access(pdict_.VarToLevelVar(i)))));
  }
  
  for(size_t i = 0; i < (pdict_.num_prules() - GetOffset(Level() - 1)); i++){
    extraction_length_[pdict_.LevelVarToVar(i) - CHARSIZE] = tmp_extraction_length[i]; 
  }
}

void ESPTree::RenameNextLevelVars(Permutation *perm_index,
				  const uint64_t kInputSpace,
				  const uint64_t kMode){
  if(kMode == 1){
    for(size_t i = 0; i < remaining_len_; i++){
      input_buffer_[i] = pdict_.LevelVarToVar(perm_index->ReverseAccess(pdict_.VarToLevelVar(input_buffer_[i])));
    }
    input_buffer_.resize(remaining_len_);
    vector<uint64_t> (input_buffer_).swap(input_buffer_);
  }
  else{

    ifstream ifs("DiskBuffer2");
    ofstream ofs("DiskBuffer1");
    
    uint64_t total_read_length = 0;
    uint64_t read_length = kInputSpace * TRANSMB;
    
    ifs.seekg(0,ios::end);
    uint64_t input_length = ifs.tellg()/sizeof(uint64_t);
    ifs.seekg(0,ios::beg);

      
    if(read_length > input_length){
      read_length = input_length;
    }
    
    while(ifs.read((char*)&input_buffer_[0], read_length * sizeof(uint64_t)) 
	  && total_read_length != input_length){
      for(size_t i = 0; i < read_length ; i++){
	input_buffer_[i] = pdict_.LevelVarToVar(perm_index->ReverseAccess(pdict_.VarToLevelVar(input_buffer_[i])));
      }
      ofs.write((char*)&input_buffer_[0], read_length * sizeof(uint64_t));
      
      total_read_length += read_length;
      if((total_read_length + read_length) > input_length){
	read_length = input_length - total_read_length;
      }
    }
    ifs.close();
    ofs.close();
  }
}

uint64_t ESPTree::GetNumPRules(){
  return pdict_.num_prules();
}

void ESPTree::SetRoot(const uint64_t kVar){
  root_ = kVar;
  rpdict_.Delete();
  pdict_.AdjustLength();
}

uint64_t ESPTree::root(){
  return root_;
}

uint64_t ESPTree::extraction_length(const uint64_t kVar){
  if(kVar < CHARSIZE){
    return 1;
  }
  else if(kVar == DUMMYCODE){
    return 0;
  }
  return extraction_length_[kVar - CHARSIZE];
}

CharMap ESPTree::char_map(){
  return char_map_;
}

uint64_t ESPTree::Level(){
  return pdict_.offset_length();
}

uint64_t ESPTree::GetOffset(const uint64_t kLevel){
  return pdict_.offset(kLevel);
}

PRule ESPTree::GetPRule(const uint64_t kIndex){
  return pdict_.prules(kIndex);
}

void ESPTree::Clear(){
  
  pdict_.Clear();
  vector<uint64_t> ().swap(extraction_length_);
  rpdict_.Delete();
  char_map_.Clear();
  root_ = 0;
  
}




