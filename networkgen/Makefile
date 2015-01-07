srcdir  = .

CC      = c++
CFLAGS  = -g -Wuninitialized -std=c++11 -O3 -fomit-frame-pointer
LIBS	=  

GENSOURCES =  ArrayFunctions.cpp\
Eliminator.cpp\
PoreNetwork.cpp \
inputParser.cpp \
main.cpp \
vtk.cpp

GENOBJECTS = $(GENSOURCES:.c=.o)

Generator: $(GENOBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	#$(CC) $(LDFLAGS) $(OBJECTS) -o $@


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm  -f $(OBJECTS)
	rm  -rf *.dSYM
	rm  -f Generator
	rm  -f results.dat
	rm 	-f results.csv
