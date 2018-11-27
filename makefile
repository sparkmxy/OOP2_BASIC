PROGRAM = code

CXX = g++
CXXFLAGS = -g -std=c++11

CPP_FILES = $(wildcard lab2/Basic/*.cpp)
H_FILES = $(wildcard lab2/Basic/*.h)

LDOPTIONS = -L.

$(PROGRAM): $(CPP_FILES) $(H_FILES) 
	$(CXX) -o $(PROGRAM) $(CXXFLAGS) $(LDOPTIONS) $(CPP_FILES)