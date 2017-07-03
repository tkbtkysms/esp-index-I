/* search_query.cppA
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

#include "search_query.hpp"

using namespace std;

void SearchQuery::CountQuery(ifstream &ifs, ifstream &pfs){

  esp_index_.Load(ifs);	
  ifs.close();
  double time = 0;
  double sum_find_evidence_time=0;
  double sum_traverse_tree_time = 0.0;
  size_t query_length = 0;
  size_t sum_query_length = 0;
  size_t sum_occ = 0;
  double count_occ_time = 0.0;
  string line;
  uint64_t c; 
  bool find = false;
  bool cflag = true;
  
  num_query_ = 0;

  query_.resize(1024);
  while(getline(pfs,line)){
    cout << "query" << num_query_ << " : ";
    query_.clear();
    evidences_.clear();
    evidences_level_.clear();

    cflag = true;
    num_query_++;
    query_length = 0;
    
    for(size_t i = 0; i < line.length();i++){
      if((c = esp_index_.GetCharToVar((uint8_t)line[i])) == DUMMYCODE){
	cflag = false;
      }
      query_length++;
      query_.push_back(c);
    }
    
    sum_query_length += query_length;
    
    if(cflag){
      time = Gettime::get_time();
      find = FindEvidences();
      sum_find_evidence_time += Gettime::get_time() - time;
      if(find){

	occount_ = 0;
	time = Gettime::get_time();
	count_occ_time += RepeatCountQuery(evidences_[core_],
					   core_,
					   num_evidences_ - core_ - 1);
	sum_traverse_tree_time += Gettime::get_time() - time;
	sum_occ += occount_;
	
	if(occount_ != 0){
	  cout << "OCC : " << occount_ << endl;
	  cout << endl;
	  cout << endl;
	}
	else{
	  cout << "This pattern does't exist" << endl;
	  cout << line << endl;
	  cout << endl;
	  cout << endl;
	}
      }
      else{
	cout << "This pattern does't exist" << endl;
	cout << line << endl;
	cout << endl;
	cout << endl;
      }
    }
    else{
      cout << "This pattern does't exist" << endl;
      cout << line << endl;
      cout << endl;
      cout << endl;
    }
    line.clear();
  }
  esp_index_.Clear();

  
  cout << "total OCC" << sum_occ << endl;
  cout.precision(8);
  cout << "Counting time : " << (sum_traverse_tree_time + sum_find_evidence_time) * 1000 << "msec" << endl;
  cout << "Counting time/Character : " << (sum_traverse_tree_time + sum_find_evidence_time) / sum_query_length * 1000 << "msec" << endl;
  cout << "Counting time/Query : " << (sum_traverse_tree_time + sum_find_evidence_time) / num_query_ * 1000 << "msec" << endl;

}


void SearchQuery::LocateQuery(ifstream &ifs,ifstream &pfs){
  
  double time = 0.0;
  size_t query_length = 0;
  size_t sum_query_length = 0;
  size_t sum_occ = 0;
  double sum_find_evidence_time=0;
  double find_position_time =0;
  double sum_traverse_tree_time = 0.0;
  string line;
  uint64_t c;
  bool find = false;
  bool cflag;

  esp_index_.Load(ifs);	
  ifs.close();
  
  num_query_ = 0;
  query_.resize(1024);
  query_.clear();
  line.clear();

  
  while(getline(pfs,line)){
    cout << "query" << num_query_ << " : ";
    cflag = true;
    location_.push_back(vector<uint64_t>());
    num_query_++;
    query_length = 0;
    
    for(size_t i = 0; i < line.length();i++){
      query_length++;
      if((c = esp_index_.GetCharToVar((uint8_t)line[i])) == DUMMYCODE){
	cflag = false;
      }
      query_.push_back(c);
    }
    sum_query_length += query_length;
    
    if(cflag){
    
      time = Gettime::get_time();
      find = FindEvidences();
      sum_find_evidence_time += Gettime::get_time() - time;
    
      if(find){
      
	occount_ = 0;
	time = Gettime::get_time();
	find_position_time += RepeatLocateQuery(evidences_[core_],
						core_,
						num_evidences_ - core_ - 1,
						0);
	sum_traverse_tree_time += Gettime::get_time() - time;
    
	sum_occ += occount_;
	if(occount_ != 0){
	  for(size_t i = 0; i < location_[num_query_-1].size();i++){
	    cout << location_[num_query_-1][i] << " " ;
	  }
          
	  cout << endl;
	 cout << "OCC: " << occount_ << endl; 
	 cout << endl;
	}
	else{
	  cout << "This pattern does't exist" << endl;
	  cout << line << endl;
	  cout << endl;
	  cout << endl;
	}
    
      }
      else{
	cout << "This pattern does't exist" << endl;
	cout << line << endl;
	cout << endl;
	cout << endl;

      }
    }
    else{
      cout << "This pattern does't exist" << endl;
      cout << line << endl;
      cout << endl;
      cout << endl;
    }
    evidences_.clear();
    line.clear();
    query_.clear();
    evidences_level_.clear();

  }

  
  cout << endl;
  esp_index_.Clear();
  cout << "Total OCC" << sum_occ << endl;
  cout.precision(8);

  cout << "Location time : " << (sum_traverse_tree_time + sum_find_evidence_time)*1000 << "msec" << endl;
  cout << "Location time/Character : " << (sum_traverse_tree_time + sum_find_evidence_time)/ sum_query_length*1000 << "msec" << endl;
  cout << "Location time/Query : " << (sum_traverse_tree_time + sum_find_evidence_time)/num_query_*1000 << "msec" << endl; 

}


bool SearchQuery::FindEvidences(){

  vector<uint64_t> suffix_evidences;
  vector<uint32_t> suffix_evidences_level;
  uint64_t num_suffix_evidences = 0;
  uint64_t query_length = query_.size();
  uint64_t st = 0;
  uint64_t nextst = 0;
  uint64_t ed = query_length - 1;
  uint64_t loop = 0;
  
  core_ = 0;
  core_extraction_length_ = 0;
  num_evidences_ = 0;
  core_extraction_length_ = 0;
  evidences_.resize(query_length);
  evidences_level_.resize(query_length);
  suffix_evidences.resize(query_length);
  suffix_evidences_level.resize(query_length);
  
  if(query_length <= 5){
    pos_ = 0;
    PushEvidence(query_length, 0);
  }
  else{
    
    while(ed > st + 4){
      next_level_len_ = 0;
      pos_ = st;
      nextst = 0;
      loop++;
      if(st != 0){
	previous_ = query_[st - 1];
      }
      else{
	previous_ = DUMMYCODE;
      }	
      
      while (ed > (pos_ + 4)){

	if(IsFirstConsistency(st)){
	  if(IsPair(query_,
		    pos_,
		    previous_)){
	    if(!PushEvidence(2, loop - 1)){
	      return false;
	    }
	    Build2Tree();
	  }
	  else{
	    if(!PushEvidence(3, loop - 1)){
	      return false;
	    }
	    Build2_2Tree();
	  }
	  nextst = next_level_len_;
	  break;
	}
	else{
	  if(IsPair(query_,
		    pos_,
		    previous_)){
	 
	    if(!PushEvidence(2, loop - 1)){
	      return false;
	    }
	    pos_ += 2;
	    previous_ = query_[pos_ - 1];
	  }
	  else{
	    if(!PushEvidence(3, loop - 1)){
       	      return false;
	    }
	    pos_ += 3;
	    previous_ = query_[pos_ - 1];
	  }
	}
      }
      while(ed > (pos_ + 4)){
	if(IsPair(query_,
		  pos_,
		  previous_)){
	  Build2Tree();
	}
	else{
	  Build2_2Tree();
	}
      }
    
      if(ed >= pos_){
	for(size_t i = ed; i >= pos_;i--){
	  if(!IsExistingVar(suffix_evidences[i])){      
	    return false;
	  }
	  suffix_evidences[num_suffix_evidences] = query_[i];
	  suffix_evidences_level[num_suffix_evidences] = loop - 1;
	  num_suffix_evidences++;
	}
      }

      st = nextst;
      
      if(next_level_len_ == 0){
	ed = 0;
      }
      else{
	ed = next_level_len_ - 1;
      }
    }
  }

  if(nextst != 0 && next_level_len_ != 0){
    if(ed >= st){
      pos_ = st;
      PushEvidence(ed - st + 1, loop);
    }
  }
  
  for(int i = num_suffix_evidences - 1; i >= 0; i--){
    pos_ = 0;
    query_[0] = suffix_evidences[i];
    if(!PushEvidence(1, suffix_evidences_level[i])){
      return false;
    }
  }
  
  if(num_evidences_ == query_length){
    core_ = query_length/2;
  }
   
  return true;

}

bool SearchQuery::IsFirstConsistency(const uint64_t kStartPos){
  uint64_t code0 = (uint64_t)query_[pos_];
  uint64_t code1 = (uint64_t)query_[pos_ + 1];
  uint64_t code2 = (uint64_t)query_[pos_ + 2];
  uint64_t code3 = (uint64_t)query_[pos_ + 3];
  uint64_t code4 = (uint64_t)query_[pos_ + 4];
  uint64_t code5 = (uint64_t)query_[pos_ + 5];
  if(pos_ > kStartPos){
    if(IsRepetition(code0, code1)){
      if(!IsRepetition(code1, code2) && !IsRepetition(code2, code3) && IsRepetition(code3, code4)){
	return true;
      }
      if(IsRepetition(code1, code2)){
	if(!IsRepetition(code2, code3) && !IsRepetition(code3, code4) && IsRepetition(code4, code5) ){
	  return true;
	}
	else if(IsRepetition(code2, code3)){
	  return false;
	}
	return true;
      }
      return true;
    }  
    if(IsRepetition(code2, code3)){
      return false;
    }
    if(IsRepetition(code3, code4)){
      return true;
    }
    if(IsRepetition(code4, code5)){
      return false;
    }
  }
  if(pos_ > (kStartPos + 4)){
    if(IsMinimal(previous_, code0, code1) || IsMaximal(query_, pos_, previous_)){
      return true;
    }
    if(IsMinimal(code0, code1, code1) || IsMaximal(query_, pos_ + 1, query_[pos_])){
      return true;
    }
  }
  return false;
}

bool SearchQuery::IsExistingVar(const uint64_t kVar){
  return kVar < esp_index_.num_prules();
}

bool SearchQuery::PushEvidence(const uint64_t kNum, const uint64_t kLevel){
  
  for(uint64_t i = 0; i < kNum; i++){  
    if(!IsExistingVar(query_[pos_ + i])){
      return false;
    }
    evidences_[num_evidences_] = query_[pos_ + i];
    FindCore();
    evidences_level_[num_evidences_++] = kLevel;
  }
  return true;

}

void SearchQuery::Build2Tree(){
  
  uint64_t var,right;
  var = esp_index_.ReverseAccessToPRule(query_[pos_], 
					(right  = query_[pos_ + 1]));
  query_[next_level_len_++] = var;
  previous_ = right;
  pos_ += 2;

}

void SearchQuery::Build2_2Tree(){
  
  uint64_t var1, var2, right;
  var1 = esp_index_.ReverseAccessToPRule(query_[pos_ + 1], 
					 (right  = query_[pos_ + 2]));
  var2 = esp_index_.ReverseAccessToPRule(query_[pos_], var1);
  query_[next_level_len_++] =  var2;
  previous_ = right;
  pos_ += 3;

}

void SearchQuery::FindCore(){

  uint64_t tmp_length;
  
  if((tmp_length = esp_index_.extraction_length(evidences_[num_evidences_])) > core_extraction_length_){
    core_ = num_evidences_;
    core_extraction_length_ = tmp_length;
  }
  
}

double SearchQuery::RepeatCountQuery(const uint64_t kVar,
				     const int kRemainingLeftLength,
				     const int kRemainingRightLength){
  int consistent_length;
  uint64_t parent_node; 
  uint64_t num_parent = 0;
  double sum_time = 0.0, time = 0.0;
  

  uint64_t first_parent_left_bit_pos = esp_index_.FirstLeftParentBitPos(kVar);
  if(first_parent_left_bit_pos != DUMMYCODE){
    while((parent_node = esp_index_.LeftParent(kVar, first_parent_left_bit_pos, num_parent)) != DUMMYCODE){
      if((consistent_length = SearchRightEvidences(esp_index_.right(parent_node), kRemainingRightLength)) != -1){
	if((kRemainingLeftLength == 0) 
	   && (kRemainingRightLength - consistent_length == 0)){
	  time = Gettime::get_time();
	  CountOcc(parent_node);
	  sum_time += Gettime::get_time() - time;
	}
	else{
	  sum_time += RepeatCountQuery(parent_node,
				       kRemainingLeftLength,
				       kRemainingRightLength - consistent_length);
	}
      }   
      num_parent++;
    }
  }
  
  num_parent =  0;
  while((parent_node = esp_index_.RightParent(kVar, num_parent)) != DUMMYCODE){
    if((consistent_length = SearchLeftEvidences(esp_index_.left(parent_node), kRemainingLeftLength)) != -1){
      if(((kRemainingLeftLength - consistent_length) == 0)
	 && (kRemainingRightLength == 0)){
	time = Gettime::get_time();
	CountOcc(parent_node);
	sum_time += Gettime::get_time() - time;
      }
      else{
	sum_time += RepeatCountQuery(parent_node,
				     kRemainingLeftLength - consistent_length,
				     kRemainingRightLength);
      }
    }    
    num_parent++;
  }
  return sum_time;
}

double SearchQuery::RepeatLocateQuery(const uint64_t kVar,
				      const int kRemainingLeftLength,
				      const int kRemainingRightLength,
				      const uint64_t kLocalLength){

  int consistent_length;
  uint64_t parent_node; 
  uint64_t tmp_local_length = 0; 
  uint64_t num_parent = 0;
  double time = 0.0, sum_time = 0.0;

  uint64_t first_parent_left_bit_pos = esp_index_.FirstLeftParentBitPos(kVar);
  if(first_parent_left_bit_pos != DUMMYCODE){
    while((parent_node = esp_index_.LeftParent(kVar, first_parent_left_bit_pos, num_parent)) != DUMMYCODE){
      if((consistent_length = SearchRightEvidences(esp_index_.right(parent_node), kRemainingRightLength)) != -1){
	if((kRemainingLeftLength == 0) 
	   && ((kRemainingRightLength - consistent_length) == 0)){
	  time = Gettime::get_time();
	  Locate(parent_node, kLocalLength);
	  sum_time += Gettime::get_time() - time;
	}
	else{
	  sum_time += RepeatLocateQuery(parent_node, 
					kRemainingLeftLength,
					kRemainingRightLength - consistent_length,
					kLocalLength);
	}
      }
      num_parent++;
    }
  }

  num_parent =  0;
  while((parent_node = esp_index_.RightParent(kVar, num_parent)) != DUMMYCODE){   
    tmp_local_length = 0;
    if((consistent_length = LocateSearchLeftEvidences(esp_index_.left(parent_node), kRemainingLeftLength, &tmp_local_length)) != -1){
      /*if(kRemainingLeftLength == 0 && esp_index_.left(parent_node)){
	tmp_local_length += esp_index_.extraction_length(esp_index_.left(parent_node));
	}*/
      if(((kRemainingLeftLength - consistent_length) == 0) 
	 && (kRemainingRightLength == 0)){
	time = Gettime::get_time();
	Locate(parent_node, kLocalLength + tmp_local_length);
	sum_time += Gettime::get_time() - time;
      }
      else{
	if(kRemainingLeftLength == 0){
	  tmp_local_length += esp_index_.extraction_length(esp_index_.left(parent_node));
	}
	sum_time += RepeatLocateQuery(parent_node,
				      kRemainingLeftLength - consistent_length,
				      kRemainingRightLength,
				      kLocalLength + tmp_local_length);
      } 
    }
    num_parent++;
  }
  return sum_time;
}

int SearchQuery::SearchRightEvidences(const uint64_t kVar ,const int kRemainingRightLength){

  if(kRemainingRightLength <= 0){
    return 0;
  }
  
  if(kVar == evidences_[num_evidences_ - kRemainingRightLength]){
    return 1;
  }
  
  uint32_t height = evidences_level_[num_evidences_ - kRemainingRightLength];
  
  if(height > 0){
    if(kVar < esp_index_.offset(height - 1)){
      return -1;
    }
  }
  else if(kVar < CHARSIZE){
      return -1;
  }

  int left_consistency, right_consistency;
  
  if((left_consistency = SearchRightEvidences(esp_index_.left(kVar), kRemainingRightLength)) == -1){
    return -1;
  }
  
  if((right_consistency = SearchRightEvidences(esp_index_.right(kVar), kRemainingRightLength - left_consistency)) == -1){
    return -1;
  }
  else{
    return left_consistency + right_consistency;
  }
}

int SearchQuery::SearchLeftEvidences(const uint64_t kVar ,const int kRemainingLeftLength){
  
  if(kRemainingLeftLength <= 0){
    return 0;
  }
  
  if(kVar == evidences_[kRemainingLeftLength - 1]){
    return 1;
  }
  
  uint32_t height = evidences_level_[kRemainingLeftLength - 1];
  
  if(height > 0){
    if(kVar < esp_index_.offset(height-1)){
      return -1;
    }
  }
  else if (kVar < CHARSIZE){
    return -1;
  }
  
  int left_consistency,right_consistency;
  
  if((right_consistency = SearchLeftEvidences(esp_index_.right(kVar),kRemainingLeftLength)) == -1){
    return -1;
  }
  
  if((left_consistency = SearchLeftEvidences(esp_index_.left(kVar),kRemainingLeftLength - right_consistency)) == -1){
    return -1;
  }
  
  return left_consistency + right_consistency;  

}

int SearchQuery::LocateSearchLeftEvidences(const uint64_t kVar,
					   const int kRemainingLeftLength, 
					   uint64_t *local_length){
  if(kRemainingLeftLength <= 0){
    return 0;
  }
  if(kVar == evidences_[kRemainingLeftLength - 1]){
    return 1;
  }
  
  uint32_t  height = evidences_level_[kRemainingLeftLength - 1];
  
  if(height > 0){
    if(kVar < esp_index_.offset(height - 1)){
      return -1;
    }
  }
  else if (kVar < CHARSIZE){
      return -1;
  }
  
  int left_consistency, right_consistency;
  
  if((right_consistency = LocateSearchLeftEvidences(esp_index_.right(kVar),kRemainingLeftLength, local_length)) == -1){
    return -1;
  }
  if((left_consistency = LocateSearchLeftEvidences(esp_index_.left(kVar), kRemainingLeftLength - right_consistency, local_length)) == -1){
    return -1;
  }
  
  if((kRemainingLeftLength - right_consistency) == 0){
    *local_length += esp_index_.extraction_length(esp_index_.left(kVar));
  }
  
  return left_consistency + right_consistency;  
}

void SearchQuery::CountOcc(const uint64_t kVar){
  
  if(kVar == esp_index_.root()){
    occount_++;
  }
  else{
    
    uint64_t parent_node;
    uint64_t num_parent = 0;
    

    uint64_t first_parent_left_bit_pos = esp_index_.FirstLeftParentBitPos(kVar);
    if(first_parent_left_bit_pos != DUMMYCODE){
      while((parent_node = esp_index_.LeftParent(kVar, first_parent_left_bit_pos, num_parent)) != DUMMYCODE){
	CountOcc(parent_node);
	num_parent++;
      }
    }
    
    num_parent =  0;
    while((parent_node = esp_index_.RightParent(kVar,num_parent))!= DUMMYCODE){
      CountOcc(parent_node);
      num_parent++;
    }
  }

}

void SearchQuery::Locate(const uint64_t kVar, const uint64_t kLocation){
  
  if(kVar == esp_index_.root()){
    occount_++;
    location_[num_query_-1].push_back(kLocation);
  }
  else{
    uint64_t parent_node;
    uint64_t num_parent = 0;
    uint64_t first_parent_left_bit_pos = esp_index_.FirstLeftParentBitPos(kVar);
    if(first_parent_left_bit_pos != DUMMYCODE){
      while((parent_node = esp_index_.LeftParent(kVar, first_parent_left_bit_pos, num_parent)) != DUMMYCODE){
	Locate(parent_node, kLocation);
	num_parent++;
      }
    }
    
    num_parent =  0;
    while((parent_node = esp_index_.RightParent(kVar,num_parent)) != DUMMYCODE){
      Locate(parent_node,
	     kLocation 
	   + esp_index_.extraction_length(esp_index_.left(parent_node)));
    num_parent++;
    }
  }
}


       
