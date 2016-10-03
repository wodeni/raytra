CXX = g++
INCLUDES = -I. -I/usr/local/include/OpenEXR -L/usr/local/lib
CXXFLAGS = -g -Wall -std=c++11 
LDFLAGS =  
LDLIBS = -lIlmImf -lImath -lHalf


prog_out: 
	$(CXX) *cc $(CXXFLAGS) $(INCLUDES) $(LDLIBS) -o prog_out 

.PHONY: clean
clean: 
	rm -rf *.o core *.dSYM prog_out
	
.PHONY: all
all: clean prog_out 
