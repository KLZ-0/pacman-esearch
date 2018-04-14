CXX=g++
CXXFLAGS=-Wall -O2 -pipe -march=westmere -std=c++14

all: esearch

esearch: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
