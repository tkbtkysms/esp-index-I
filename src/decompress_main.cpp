/* decopress_main.cpp
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

#include<cstdlib>

#include "esp_index.hpp"
#include "cmdline.h"
#include "get_time.hpp"

using namespace std;

int main(int argc,char** argv){

  cmdline::parser p;
  p.add<string>("input_file",  'i', "index file name",  true);
  p.add<string>("output_file", 'o', "output file name", true);
  p.parse_check(argc, argv);
  
  string input_file   = p.get<string>("input_file");
  string output_file  = p.get<string>("output_file");

  ifstream ifs(input_file.c_str());
  ofstream ofs(output_file.c_str());
  double time;
  cout <<"start decompression" << endl;
  ESPIndex eidx;
  
  time = Gettime::get_time();
  eidx.Decompress(ifs,ofs);
  time = Gettime::get_time() - time;
  cout <<"end decompression" << endl;
  cout << "Decompression time: "<< time <<" sec"<< endl;
  ifs.close();
  ofs.close();
}
