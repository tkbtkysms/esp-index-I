/* esp_index.hpp
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

#ifndef ESPINDEX_HPP
#define ESPINDEX_HPP

#include <cstdint>
#include "get_time.hpp"
#include "esp_tree.hpp"
#include "RSDic.hpp"
#include "RSDicBuilder.hpp"
#include "gmr.hpp"

class ESPIndex{

public:
  ESPIndex(): left_(),
	      right_(),
	      sparse_bit_(),
	      extraction_length_(),
	      offset_(),
	      root_(),
	      num_prules_(),
	      level_(),
	      char_map_(){}
  ~ESPIndex() {}
  void Build(ESPTree &est_tree,
	     const uint32_t kPositionRatio,
	     std::ofstream &ofs);
  
  uint64_t left(const uint64_t kVar);
  uint64_t right(const uint64_t kVar);
  uint64_t FirstLeftParentBitPos(const uint64_t kIndex);
  uint64_t LeftParent(const uint64_t kIndex,
		      const uint64_t kFirstParentLeftBitPos,
		      const uint64_t kParentNum);
  uint64_t RightParent(const uint64_t kIndex,const uint64_t kParentNum);
  uint64_t ReverseAccessToPRule(const uint64_t kLeft,const uint64_t kRight);
  
  uint64_t extraction_length(const size_t kIndex);
  
  uint64_t offset(const size_t kIndex);
  
  uint64_t root();
  
  uint64_t num_prules();
  
  uint64_t GetCharToVar(const char c);
  char GetVarToChar(const uint64_t index);
  
  void RepeatDecompress(const uint64_t index,
			std::ofstream &ofs, 
			std::vector<uint64_t> &tmp_left, 
			std::vector<uint64_t> &tmp_right);
  void Decompress(std::ifstream &ifs,std::ofstream &ofs); 
  
  void Save(std::ofstream &ofs);
  void Load(std::ifstream &ifs);
  

private:
  void SetOffset(ESPTree &esp_tree); 
  void SetESPTreeInfo(ESPTree &esp_tree);
  void BuildExtractionLength(ESPTree &esp_tree, const uint32_t kPositionRatio);
  void BuildLeft(ESPTree &esp_tree);
  void BuildRight(ESPTree &esp_tree);

private:
  rsdic::RSDic left_;
  GMR right_;  
  rsdic::RSDic sparse_bit_;
  FFLCArray extraction_length_;
  FFLCArray offset_;
  uint64_t root_;
  uint64_t num_prules_;
  uint64_t level_;
  CharMap char_map_;  
};

#endif //ESPINDEX_HPP















