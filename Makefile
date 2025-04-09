CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra

all: cos_distance test

cos_distance: main.cpp CosineDistance.h
	$(CXX) $(CXXFLAGS) -o cos_distance main.cpp

test: test.cpp CosineDistance.h doctest.h
	$(CXX) $(CXXFLAGS) -o test test.cpp

clean:
	rm -f cos_distance test