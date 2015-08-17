/* search_query.hpp
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

#ifndef SEARCH_QUERY_HPP
#define SEARCH_QUERY_HPP

#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include "constant_numbers.hpp"
#include "is_pair.hpp"
#include "get_time.hpp"
#include "esp_index.hpp"
#include "esp_tree.hpp"
#include "phrase_dictionary.hpp"

class SearchQuery{

public:
  SearchQuery(): esp_index_(),
                 evidences_(),
		 evidences_level_(),
		 location_(),
		 query_(),
		 num_evidences_(),
		 occount_(),
		 core_(),
		 core_extraction_length_(),
		 pos_(),
		 previous_(),
		 next_level_len_(),
		 num_query_(){}
  ~SearchQuery(){}
  
  void CountQuery(std::ifstream &ifs, std::ifstream &pfs);
  void LocateQuery(std::ifstream &ifs, std::ifstream &pfs);

private:
  bool FindEvidences();
  bool IsFirstConsistency(const uint64_t kStart);
  bool IsExistingVar(const uint64_t kVar);
  bool PushEvidence(const uint64_t kNum, const uint64_t kHeight);
  void Build2Tree();
  void Build2_2Tree();
  void FindCore(); 

  double RepeatCountQuery(const uint64_t kVar,
			  const int kRemainingLeftLength,
			  const int kRemainingRightLength);
  double RepeatLocateQuery(const uint64_t kVar,
			   const int kRemainingLeftLength,
			   const int kRemainingRightLength,
			   const uint64_t kLocalPos);
  int SearchLeftEvidences(const uint64_t kVar,
			  const int kRemainingLeftLength);
  int SearchRightEvidences(const uint64_t kVar,
			   const int kRemainingRightLength);
  int LocateSearchLeftEvidences(const uint64_t kVar, 
				const int kRemainigLeftLength, 
				uint64_t *local_pos);
  void CountOcc(const uint64_t kVar);
  void Locate(const uint64_t kVar,
	      const uint64_t kPosition);

private:
  ESPIndex esp_index_;  
  std::vector<uint64_t> evidences_;
  std::vector<uint32_t> evidences_level_;
  std::vector<std::vector<uint64_t> > location_;
  std::vector<uint64_t> query_;
  uint64_t num_evidences_;
  uint64_t occount_;
  uint64_t core_;
  uint64_t core_extraction_length_;
  uint64_t pos_;
  uint64_t previous_;
  uint64_t next_level_len_;
  uint64_t num_query_;

};

#endif //SEARCH_QUERY_HPP




