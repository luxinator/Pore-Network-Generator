
CXX	 := clang++
CXXFLAGS := -g -O3 -std=c++0x -march=native -fomit-frame-pointer 
#CXXFLAGS := -g -Wuninitialized -std=c++0x -Wall
INCLUDES :=					# This would contain any -I options to the
  							# compiler, if there are any.
LIBS     := 				# Contains libraries we need to link in.
OBJECTS  := ArrayFunctions.o Eliminator.o PoreNetwork.o inputParser.o main.o vtk.o writerFunctions.o
   
Generator: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LIBS)
	cp Generator ../Generator


%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

clean:
	rm  -f $(OBJECTS)
	rm  -rf *.dSYM
	rm  -f Generator ../Generator
