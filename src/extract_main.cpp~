/* extrat_main.cpp
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

#include <cstdlib>
#include "extract.hpp"
#include "cmdline.h"

using namespace std;

int main(int argc, char** argv){
  
  cmdline::parser p;
  
  p.add<string>("index_file", 'i', "index file name",true);
  p.add<string>("position_file", 'p', "position file name",true);
  p.parse_check(argc,argv);
  
  string input_file = p.get<string>("index_file");
  string position_file = p.get<string>("position_file");
  
  ifstream ifs(input_file.c_str());
  ifstream pfs(position_file.c_str());
  
  if(!ifs || !pfs){
    cerr << "input file or position file error" << endl;
    exit(1);
  }
 
  Extract ext;
  ext.StartExtract(ifs,pfs);
  
  return 0;
}
