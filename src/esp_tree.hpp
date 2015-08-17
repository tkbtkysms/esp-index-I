/* esp_tree.hpp
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

#ifndef ESP_TREE_HPP
#define ESP_TREE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdint>
#include "constant_numbers.hpp"
#include "phrase_dictionary.hpp"
#include "reverse_phrase_dictionary.hpp"
#include "char_map.hpp"
#include "full_fixed_length_code_array.hpp"
#include "get_time.hpp"
#include "is_pair.hpp"
  

class ESPTree{
  
public:
  ESPTree() : pdict_(),
	      extraction_length_(),
	      rpdict_(),
	      char_map_(),
	      root_(),
              input_buffer_(),
              input_buffer_pos_(),
              previous_(),
              next_level_len_(),
              remaining_len_(){}
  ~ESPTree(){}
  void Build(std::ifstream &input);
  void DiskBuild(std::ifstream &input, const uint64_t kInputSpace);
  void Init(const uint64_t kLen);
  
  uint64_t GetNumPRules();
  PRule GetPRule(const uint64_t kVar);
  uint64_t Level();
  uint64_t GetOffset(const uint64_t kLevel);
  
  uint64_t extraction_length(const uint64_t kVar);
  
  CharMap char_map();

  void SetRoot(const uint64_t kRoot);
  uint64_t root();

  void Clear();
  uint64_t ByteSize(){
    return pdict_.ByteSize()
      + rpdict_.ByteSize()
      + char_map_.ByteSize()
      + Level()*sizeof(uint64_t)
      + 2*sizeof(uint64_t);
  }

private:
  void ReadInputText(std::ifstream &ifs, const uint64_t kLen);
  void Build2Tree();
  void Build2_2Tree();
  void RenameExtractionLength(Permutation *perm_index);
  void RenameNextLevelVars(Permutation *perm_index,
			   const uint64_t kInputSpace,
			   const uint64_t kMode);

private:
  PDict pdict_;
  std::vector<uint64_t> extraction_length_;
  RPDict rpdict_;
  CharMap char_map_;
  uint64_t root_;
  std::vector<uint64_t> input_buffer_;
  uint64_t input_buffer_pos_;
  uint64_t previous_;
  uint64_t next_level_len_;
  uint64_t remaining_len_;

};

#endif //ESP_TREE_HPP
