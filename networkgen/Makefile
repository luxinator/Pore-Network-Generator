
CXX	:= c++
CXXFLAGS := -g -Wuninitialized -std=c++11 -Wall
INCLUDES :=					# This would contain any -I options to the
  								# compiler, if there are any.
LIBS     := 					# Contains libraries we need to link in.
OBJECTS  := ArrayFunctions.o Eliminator.o PoreNetwork.o inputParser.o main.o vtk.o
   
Generator: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LIBS)
    
%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

clean:
	rm  -f $(OBJECTS)
	rm  -rf *.dSYM
	rm  -f Generator
	rm -f *.o