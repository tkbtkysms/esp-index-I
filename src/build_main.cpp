/* build_main.cpp
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

#include<iostream>
#include<fstream>
#include<cstdlib>
#include "esp_tree.hpp"
#include "esp_index.hpp"
#include "cmdline.h"
#include "get_time.hpp"

using namespace std;

int main(int argc,char **argv){

  cmdline::parser p;

  p.add<string>("input_file",  'i', "input file name",  true);
  p.add<string>("output_file", 'o', "index file name", true);
  p.add<uint32_t>("extraction_length_ratio", 'e', "store extracation length of nodes per ESP tree level e", false, 2);
  p.add<uint32_t>("mode", 'm', "1: read whole input string on working memory and build ESP tree\n\t\t\t\t   2: build ESP tree while reading input characters(variables) w by w mega length", false, 1);
  p.add<uint64_t>("input_space", 'w', "input space is limited by w mega length in mode 2", false, 10ull);

  p.parse_check(argc, argv);

  string input_file   = p.get<string>("input_file");
  string output_file  = p.get<string>("output_file");
  uint32_t mode = p.get<uint32_t>("mode");
  uint32_t pr = p.get<uint32_t>("extraction_length_ratio");
  uint64_t w = p.get<uint64_t>("input_space");
  
  ESPTree esp_tree;
  ESPIndex esp_index;
  double time;
  
  if(mode == 1){
    time = Gettime::get_time();
    ifstream ifs(input_file.c_str());
    esp_tree.Build(ifs);
    ofstream ofs(output_file.c_str());
    esp_index.Build(esp_tree,
		    pr,
		    ofs);
    time = Gettime::get_time() - time;
  }
  else if (mode == 2){

    time = Gettime::get_time();
    ifstream ifs(input_file.c_str());
    esp_tree.DiskBuild(ifs, w);
    ofstream ofs(output_file.c_str());
    esp_index.Build(esp_tree,
		    pr,
		    ofs);
  }
  else{
    cerr << "mode error" << endl;
    exit(1);
  }

  cout << "Build index time: " << time << " sec" << endl; 

  return 0;
}
