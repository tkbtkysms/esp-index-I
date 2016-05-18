/* extract.cpp
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

#include "extract.hpp"

using namespace std;

void Extract::StartExtract(ifstream &ifs, ifstream &pfs){
  
  esp_index_.Load(ifs);
  ifs.close();
  
  double time;
  uint64_t start_pos, end_pos;
  uint64_t start = esp_index_.root(); 
  size_t length = 0;
  size_t error_num = 0;
  string line;
  stringstream ss;
 
  num_pos_ = 0;

  time = Gettime::get_time();

  line.clear();
  substr_.clear();

  while(getline(pfs,line)){
    
    cout << "Query"  << num_pos_ << " : ";

    num_pos_++;
    substr_.push_back(string());


    ss << line;
    ss >> start_pos;
    ss >> end_pos;

    
    cout << start_pos << " " << end_pos << endl;
    cout << endl;
    
    ss.clear();
    pos_ = 0;
    
    if(start_pos < end_pos && end_pos <= esp_index_.extraction_length(start)){
      len_ = end_pos - start_pos + 1;
      length += len_;
      RepeatExtract(esp_index_.root(),
		    start_pos,
		    end_pos); 

      cout << substr_[num_pos_ - 1] << endl;

    }
    else{
      cerr << "error" << endl;
      error_num++;
    }
    cout << endl;
    line.clear();
  }
  
  num_pos_ -= error_num;
  time = Gettime::get_time() - time;
  cout.precision(8);
  cout << "Extraction time : " << time*1000 << " msec" << endl;
  cout << "Extraction time/Character : " << time/length << " msec" << endl;
  cout << "Extraction time/Pattern : " << time/num_pos_*1000 << " msec" <<endl;
}

void Extract::RepeatExtract(const uint64_t node,const uint64_t start_pos, const uint64_t end_pos){
  uint64_t left,right, left_length;
  if(node >= CHARSIZE){
    left = esp_index_.left(node);
    right = esp_index_.right(node);
    if(left > CHARSIZE){
      left_length = esp_index_.extraction_length(left);
    }
    else{
      left_length = 1;
    }   
    if(start_pos < left_length){
      RepeatExtract(left,start_pos,end_pos);
      RepeatAddChar(right);
    }
    else{
      RepeatExtract(right,start_pos - left_length,end_pos -left_length);
    }
  }
  else{    
    len_--;
    cout << node << endl;
    substr_[num_pos_ - 1].append(1, esp_index_.GetVarToChar(node));
    pos_++;
  }
    
}

void Extract::RepeatAddChar(const uint64_t node){
  if(len_ != 0){
    if(node < CHARSIZE){
      len_--;
      substr_[num_pos_ - 1].append(1, esp_index_.GetVarToChar(node));
      pos_++;
    }
    else{
      RepeatAddChar(esp_index_.left(node));
      RepeatAddChar(esp_index_.right(node));
    }
  }
}
