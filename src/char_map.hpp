/* char_map.hpp
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

#ifndef CHAR_MAP_HPP
#define CHAR_MAP_HPP

#include "constant_numbers.hpp"
#include <cstdint>

class CharMap{
private:
  std::vector<uint8_t> char_to_var_;
  std::vector<char> var_to_char_;
  bool *is_char_;
  uint64_t char_size_;
  
public:
  CharMap() : char_to_var_(CHARSIZE),
	      var_to_char_(CHARSIZE),
	      is_char_(),
	      char_size_() {
    is_char_ = new bool[CHARSIZE];
    for(size_t i = 0; i < CHARSIZE; i++){
      is_char_[i] = false;      
    }
    char_size_ = 0;
  }
  
  void SetChar(const char kChar){
    char_to_var_[(uint8_t)kChar] = char_size_;
    var_to_char_[char_size_] = kChar;
    is_char_[(uint8_t)kChar] = true;
    char_size_++;
  }
  
  uint64_t char_to_var(const char kChar){
    if(!is_char_[(uint8_t)kChar]){
      SetChar(kChar);
    }
    return (uint64_t)char_to_var_[(uint8_t)kChar];
  }
  
  char var_to_char(const uint64_t var){
    return var_to_char_[var];
  }

  uint64_t ByteSize(){
    return CHARSIZE * sizeof(uint8_t) 
      + CHARSIZE * sizeof(char)
      + CHARSIZE 
      + sizeof(uint64_t);
  }
  
  void Clear(){
    std::vector<uint8_t> ().swap(char_to_var_);
    std::vector<char> ().swap(var_to_char_);
    delete [] is_char_;
    char_size_ = 0;;
  }
  
  void Save(std::ofstream &ofs){
    ofs.write((char*)&char_to_var_[0], sizeof(uint64_t) * CHARSIZE);
    ofs.write((char*)&var_to_char_[0], sizeof(char) * CHARSIZE);
    ofs.write((char*)&is_char_[0], sizeof(bool) * CHARSIZE);
    ofs.write((char*)&char_size_, sizeof(uint64_t));
  }
  
  
  void Load(std::ifstream &ifs){
    ifs.read((char*)&char_to_var_[0], sizeof(uint64_t) * CHARSIZE);
    ifs.read((char*)&var_to_char_[0], sizeof(char) * CHARSIZE);
    ifs.read((char*)&is_char_[0], sizeof(bool) * CHARSIZE);
    ifs.read((char*)&char_size_, sizeof(uint64_t));
  }
};

#endif //CHAR_MAP_HPP
