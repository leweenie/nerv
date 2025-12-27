CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

all: proc_reader

proc_reader: src/proc_reader.cpp
	$(CXX) $(CXXFLAGS) src/proc_reader.cpp -o build/proc_reader

run: proc_reader
	./build/proc_reader

clean:
	rm -f build/proc_reader
