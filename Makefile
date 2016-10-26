# @authar: Wode "Nimo" Ni
# @version: 2016/10/0/

CC = g++ # This is for the linking process to work correctly
CXX = g++
INCLUDES = -I. -isystem /usr/local/include/OpenEXR 
CXXFLAGS = -g -Wall -std=c++11 ${INCLUDES}
LDFLAGS = -g  -L/usr/local/lib
LDLIBS = -lIlmImf -lImath -lHalf
 
raytra: raytra.o parse.o surface.o material.o basemath.o 

raytra.o: raytra.cc material.h basemath.h ray.h
  
parse.o: parse.cc parse.h basemath.h raytra.h
  
surface.o: surface.cc surface.h basemath.h ray.h
  
material.o: material.cc material.h basemath.h
  
basemath.o: basemath.cc basemath.h
  
.PHONY: clean
clean: 
	rm -rf *.o core *.exr *.dSYM raytra
	
.PHONY: all
all: clean raytra

# hard-coded version
.PHONY: hardcoded
hardcoded:
	g++ -O3 *.cc -I. -I/usr/local/include/OpenEXR -L/usr/local/lib -lIlmImf -lImath -lHalf -Wall -std=c++11 -o raytra
