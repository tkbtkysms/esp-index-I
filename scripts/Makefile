CXX = g++
CXXFLAGS = -std=gnu++11 -O3 -W -Wall 
LINKFLAGS = -lm

SRCS =\
	cut_pattern.cpp\

OBJS = $(SRCS:%.cpp=%.o)

all: cut-pattern

cut-pattern: $(OBJS) 
	$(CXX) $(CXXFLAGS) $(OBJS) $(LINKFLAGS) -o cut-pattern

debug:
	make all CXXFLAGS="-ggdb -W -Wall -pedantic -I./libcds/include"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 

clean:
	rm -f cut-pattern *.o *~


