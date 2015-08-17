/* phrase_dictionary.hpp
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

#ifndef PHRASE_DICTIONARY_HPP
#define PHRASE_DICTIONARY_HPP

#include "constant_numbers.hpp"
#include "permutation.hpp"


class ProductionRule{
public:
  uint64_t left_;
  uint64_t right_;
  ProductionRule(): left_(),
		    right_(){}
  ProductionRule(const uint64_t kLeft, 
		 const uint64_t kRight) : left_(kLeft), 
					  right_(kRight){}
  ~ProductionRule(){}
};

typedef ProductionRule PRule;

class PhraseDictionary{
public:
  PhraseDictionary() : num_prules_(0), 
		       prules_(0),
                       offset_length_(0),
                       offset_(0){}
  ~PhraseDictionary(){}
  
  void Init(const uint64_t kLen);

  
  uint64_t num_prules(); 
  
  void PushPRule(const uint64_t kLeft, 
		 const uint64_t kRight);
  void SetPRule(const uint64_t kVar,
		const PRule kPRule);
  PRule prules(const uint64_t kVar);
  bool IsEqualPRule(const size_t kVar, 
		    const size_t kLeft, 
		    const size_t kRight);
  Permutation *Sort();

  void SetOffset();
  uint64_t offset_length();
  uint64_t offset(const uint64_t kLevel);
  uint64_t VarToLevelVar(const uint64_t kVar);
  uint64_t LevelVarToVar(const uint64_t kLevelvar);
  
  void AdjustLength();
  void Clear();
  uint64_t ByteSize();
  
private:
  void QSort(FFLCArray &array,
	     const uint64_t kStart,
	     const uint64_t kEnd);
  int ComparePRule(const uint64_t kVar1,
		   const uint64_t kVar2);
  void SortCheck();
  
private:
  uint64_t num_prules_;
  std::vector<PRule> prules_;
  uint64_t offset_length_;
  std::vector<uint64_t> offset_;
  

};

typedef PhraseDictionary PDict;

#endif
