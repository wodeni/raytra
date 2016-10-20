# @authar: Wode "Nimo" Ni
# @version: 2016/10/0/

CC = g++ # This is for the linking process to work correctly
CXX = g++
INCLUDES = -I. -isystem /usr/local/include/OpenEXR 
CXXFLAGS = -g -Wall -std=c++11 ${INCLUDES}
LDFLAGS = -g  -L/usr/local/lib
LDLIBS = -lIlmImf -lImath -lHalf


raytra: raytra.o parse.o surface.o material.o basemath.o 

raytra.o: raytra.cc 

parse.o: parse.cc 

surface.o: surface.cc 

material.o: material.cc 

basemath.o: basemath.cc

.PHONY: clean
clean: 
	rm -rf *.o core *.exr *.dSYM raytra
	
.PHONY: all
all: clean raytra

# hard-coded version
.PHONY: hardcoded
hardcoded:
	${CXX} *.cc ${CXXFLAGS} ${LDFLAGS} ${LDLIBS} -o raytra
