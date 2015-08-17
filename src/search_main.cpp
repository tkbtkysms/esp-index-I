/* search_main.cpp
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
#include <cstdlib>
#include "cmdline.h"

using namespace std;

int main(int argc, char** argv){
  cmdline::parser p;
  p.add<string>("index_file", 'i', "index file name",true);
  p.add<string>("query_file", 'q', "query file name",true);
  p.add<string>("mode", 'm' , "mode \"count\" or \"locate\"",true);
  p.parse_check(argc,argv);
  string input_file = p.get<string>("index_file");
  string pattern_file = p.get<string>("query_file");
  string mode = p.get<string>("mode");
  
  ifstream ifs(input_file.c_str());
  ifstream pfs(pattern_file.c_str());
  
  if(!ifs || !pfs){
    cerr << "error" << endl;    exit(1);
  }

  SearchQuery st;
  if(mode == "count"){
    st.CountQuery(ifs,pfs);
  }
  else if(mode == "locate"){
    st.LocateQuery(ifs,pfs);
  }
  else{
    
    cerr <<"error"<< endl;
    exit(1);

  }
 
  return 0;
}
