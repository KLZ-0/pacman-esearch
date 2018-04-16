TARGET = esearch

INSTALL = install
# CXX = g++ -pthread
CXXFLAGS = -Wall -O2 -pipe -march=x86-64 -std=c++11



all: $(TARGET)

$(TARGET): main.o Database.o
	$(CXX) $^ -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

Database.o: Database.cpp Database.h
	$(CXX) $(CXXFLAGS) -c $<

install:
	$(INSTALL) -m 0755 $(TARGET) /usr/bin/$(TARGET)
	$(INSTALL) -m 0755 eupdatedb.sh /usr/bin/eupdatedb

uninstall:
	rm -f /usr/bin/$(TARGET)
	rm -f /usr/bin/eupdatedb

clean:
	rm -f *.o $(TARGET)
