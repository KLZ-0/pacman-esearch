TARGET=esearch

INSTALL=install

# CXX=g++
CXXFLAGS=-Wall -O2 -pipe -march=x86-64 -std=c++11

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

install:
	$(INSTALL) -m 0755 $(TARGET) /usr/bin/$(TARGET)
	$(INSTALL) -m 0755 eupdatedb.sh /usr/bin/eupdatedb

uninstall:
	rm -f /usr/bin/$(TARGET)
	rm -f /usr/bin/eupdatedb

clean:
	if [ -e $(TARGET) ]; then rm $(TARGET); fi
