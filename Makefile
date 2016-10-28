# @authar: Wode "Nimo" Ni
# @version: 2016/10/0/

CC = g++ # This is for the linking process to work correctly
CXX = g++
ifeq (${USER}, niw)
INCLUDEDIR = /usr/local/include/OpenEXR
LIBDIR = /usr/local/lib
else
INCLUDEDIR = /usr/include/OpenEXR
LIBDIR = /usr/lib
endif
INCLUDES = -I. -isystem /usr/local/include/OpenEXR 
CXXFLAGS = -g -Wall -std=c++11 ${INCLUDES}
LDFLAGS = -g  -L${LIBDIR}
LDLIBS = -lIlmImf -lImath -lHalf
TAR = raytra_backup

raytra: raytra.o parse.o surface.o material.o basemath.o 

raytra.o: raytra.cc material.h basemath.h ray.h

parse.o: parse.cc parse.h basemath.h raytra.h

surface.o: surface.cc surface.h basemath.h ray.h

material.o: material.cc material.h basemath.h

basemath.o: basemath.cc basemath.h

.PHONY: clean
clean: 
	rm -rf *.o core *.dSYM raytra
	if [ -f *.exr ]; then mv *.exr outputs/; fi

.PHONY: all
all: clean raytra

# hard-coded version
.PHONY: hardcoded
hardcoded:
	g++ -O3 *.cc -I. -I${INCLUDEDIR} -L${LIBDIR} -lIlmImf -lImath -lHalf -Wall -std=c++11 -o raytra

.PHONY: tar
tar:
	if [ -d ${TAR} ]; then rm -rf ${TAR}; fi
	mkdir ${TAR}
	cp *.cc *.h ${TAR}/
	if [ -f ../tarfiles/${TAR}.tar ]; then rm ../tarfiles/${TAR}.tar; fi
	tar czvf ../tarfiles/${TAR}.tar ${TAR}
	rm -rf ${TAR}
