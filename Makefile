TARGET=esearch

INSTALL=install

CXX=g++
CXXFLAGS=-Wall -O2 -pipe -march=westmere -std=c++14

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

install:
	$(INSTALL) -m 0755 $(TARGET) /usr/bin/$(TARGET)
	$(INSTALL) -m 0755 eupdatedb /usr/bin/eupdatedb
