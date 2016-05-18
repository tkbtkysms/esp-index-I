CXX = g++
CXXFLAGS = -std=c++11 -O3 -W -Wall -I.
LINKFLAGS = -lm

RSDIC =\
	EnumCoder.cpp\
	RSDic.cpp\
	RSDicBuilder.cpp\

SRCS1 =\
	full_fixed_length_code_array.cpp\
	permutation.cpp\
	gmr.cpp\
	phrase_dictionary.cpp\
	reverse_phrase_dictionary.cpp\
	esp_tree.cpp\
	esp_index.cpp\

SRCS2 =\
	build_main.cpp \

SRCS3 =\
	decompress_main.cpp\

SRCS4 =\
	search_query.cpp\
	search_main.cpp\

SRCS5 =\
	extract.cpp\
	extract_main.cpp\

OBJRSDIC = $(RSDIC:%.cpp=%.o)	
OBJS1 = $(SRCS1:%.cpp=%.o)
OBJS2 = $(SRCS2:%.cpp=%.o)
OBJS3 = $(SRCS3:%.cpp=%.o)	
OBJS4 = $(SRCS4:%.cpp=%.o)
OBJS5 = $(SRCS5:%.cpp=%.o)

all: build-index decompress search extract

build-index: $(OBJRSDIC) $(OBJS1) $(OBJS2) 
	$(CXX) $(CXXFLAGS) $(OBJRSDIC) $(OBJS1) $(OBJS2) $(LINKFLAGS) -o build-index $(LIB)

decompress: $(OBJRSDIC) $(OBJS1) $(OBJS3)
	$(CXX) $(CXXFLAGS) $(OBJRSDIC) $(OBJS1) $(OBJS3) $(LINKFLAGS) -o decompress $(LIB)

search:  $(OBJRSDIC) $(OBJS1) $(OBJS4)
	$(CXX) $(CXXFLAGS) $(OBJRSDIC) $(OBJS1) $(OBJS4) $(LINKFLAGS) -o search $(LIB)

extract:  $(OBJRSDIC) $(OBJS1) $(OBJS5)
	$(CXX) $(CXXFLAGS) $(OBJRSDIC) $(OBJS1) $(OBJS5) $(LINKFLAGS) -o extract $(LIB)

debug:
	make all CXXFLAGS="-O0 -std=c++11 -ggdb -W -Wall -pedantic -I."

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f build-index decompress search extract *.o *~


