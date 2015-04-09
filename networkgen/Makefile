
CXX	 := clang++
#CXXFLAGS := -g -O3 -std=c++11 -march=native -fomit-frame-pointer 
CXXFLAGS := -g -Wuninitialized -std=c++11 -Wall 
INCLUDES :=					# This would contain any -I options to the
  							# compiler, if there are any.
LIBS     := 				# Contains libraries we need to link in.
SRC      := $(wildcard *.cpp)
OBJECTS  := $(SRC:.cpp=.o) #ArrayFunctions.o Eliminator.o PoreNetwork.o inputParser.o main.o vtk.o writerFunctions.o Combinator.o

Generator: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LIBS)
	cp Generator ../Generator

all: Generator

%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm  -f $(OBJECTS)
	rm  -rf *.dSYM
	rm  -f Generator ../Generator
