/* reverse_phrase_dictionary.hpp
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

#ifndef REVERSE_PHRASE_DICTIONARY_HPP
#define REVERSE_PHRASE_DICTIONARY_HPP

#include <cstdint>
#include "constant_numbers.hpp"
#include "phrase_dictionary.hpp"

class ReversePhraseDictionary{

public :
  ReversePhraseDictionary() : hash_size_(0), 
			      primes_index_(0), 
			      hash_table_(0), 
			      hash_list_(0) {}
  
  void Init(const size_t kReserveLength);
  
  uint64_t GetHashSize();
  
  uint64_t ReverseAccessToPRule(PDict &pdict,
				const uint64_t kLeft, 
				const uint64_t kRight);
  
  static uint64_t HashVal(const uint64_t kHashSize,
			  const uint64_t kLeft,
			  const uint64_t kRight);
  void Clear(PDict &pdict);
  void Delete();
  uint64_t ByteSize();
  
  
private:
  void SetValue(const uint64_t kKey,
		const uint64_t kValue,
		const bool kIsTable,
		PDict &pdict);
  uint64_t GetValue(const uint64_t kKey,const bool kIsTable);
  void ReHash(PDict &pdict);
  void Resize();

private : 
  uint64_t hash_size_;
  uint64_t primes_index_;
  std::vector<uint64_t> hash_table_;
  std::vector<uint64_t> hash_list_;
}; 
 
typedef ReversePhraseDictionary RPDict;

#endif //REVERSE_PHRASE_DICTIONARY_HPP
