abcDir    := $(HOME)/Applications/AbcBerkeley/abc
abcSrcDir := $(abcDir)/src
abcLibDir := $(abcDir)
abcLib    := libabc.a
abcFlags  := $(shell ./getAbcFlags.sh $(abcDir))

PROG      := rpgen
CXX       := g++
LD        := g++
INC       := -I$(abcSrcDir)
CXXFLAGS  := $(INC) $(abcFlags)
LDFLAGS   := -L$(abcLibDir) #-l$(abcLib)

all: $(PROG)

$(PROG): main.o $(abcLibDir)/$(abcLib)
	$(LD) $(LDFLAGS) -o $@ $<

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm *.o $(PROG)
