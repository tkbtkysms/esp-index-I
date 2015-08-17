#include<fstream>
#include<iostream>
#include<string>
#include<iterator>
#include<unordered_map>
#include<random>
#include<stdint.h>
#include"cmdline.h"

using namespace std;

int main(int argc, char ** argv){
  
  cmdline::parser p;
  
  p.add<string>("input_file", 'i', "input file name", false);
  p.add<string>("output_query_file", 'o', "output query file name", true);
  p.add<string>("forbitten_character", 'f', "forbitten character", false);
  p.add<string>("mode", 'm', "pattern/position", true,"pattern");
  p.add<uint64_t>("query_length", 'l', "query length", false, 100);
  p.add<uint64_t>("num_query", 'n', "# of queries" , false, 1000);
  
  p.parse_check(argc,argv);
  string input_file = p.get<string>("input_file");
  string output_query_file = p.get<string>("output_query_file");
  string fb_char = p.get<string>("forbitten_character");
  string mode = p.get<string>("mode");
  uint64_t query_length = p.get<uint64_t>("query_length");
  uint64_t num_query = p.get<uint64_t>("num_query");

  cout << fb_char.size() << endl;
  ifstream ifs(input_file.c_str());
  ofstream ofs(output_query_file.c_str());
  
  uint64_t input_length;
  
  
  ifs.seekg(0,ios::end);
  input_length = ifs.tellg();
  ifs.seekg(0,ios::beg);

  unordered_map<uint64_t,uint64_t> position_map;
  
  random_device rd;

  
  std::mt19937 mt(rd());

  std::uniform_int_distribution<uint64_t> dice(0,input_length - query_length -1);
  

  if(mode == "position"){
    for(uint64_t i = 0; i < num_query; i++){
      while(true){
	uint64_t st = dice(mt);
	if(position_map.find(st) == position_map.end()){
	  ofs << st << " " << st + query_length << endl;
	  position_map[st] = i;
	  break;
	}
      }
    }
  }
  else if(mode == "pattern"){

    string input((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    bool is_br;
    for(uint64_t i = 0; i < num_query; i++){
      while(true){
	uint64_t st = dice(mt);
	is_br = false;
	if(position_map.find(st) == position_map.end()){
	  for(uint64_t i = st; i < st + query_length; i++){
	    if(input[i] == '\n'){
	      is_br = true;
	      break;
	    }
	  }
	  for(uint64_t i = st; i < st + query_length; i++){
	    for(uint64_t j = 0 ; j < fb_char.size(); j++){
	      if(input[i] == fb_char[j]){
		is_br = true;
		break;
	      }
	    }
	    if(is_br) break;
	  }
	  if(!is_br){
	    for(uint64_t i = st; i < st + query_length; i++){
	      ofs << input[i];
	    }
	    ofs << "\n";
	    
	    position_map[st] = i;

	    break;
	  }
	}
      }
    }
  }
  else{
    cerr << "error" << endl;
    exit(1);
  }

  ofs.close();
  return 0;
}
