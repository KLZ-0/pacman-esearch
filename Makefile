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
	$(INSTALL) -m 0755 $(TARGET) /usr/local/bin/$(TARGET)
	$(INSTALL) -m 0755 eupdatedb.sh /usr/local/bin/eupdatedb

uninstall:
	rm -f /usr/local/bin/$(TARGET)
	rm -f /usr/local/bin/eupdatedb

clean:
	rm -f *.o $(TARGET)
