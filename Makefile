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
LDFLAGS   := -lreadline -lpthread -ldl -lm

all: $(PROG)

$(PROG): main.o ResourceAwarePatchGeneration.o write_patch.o interpolation.o $(abcLibDir)/$(abcLib) 
	$(LD) $(LDFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

ResourceAwarePatchGeneration.o: ResourceAwarePatchGeneration.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

write_patch.o: write_patch.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

interpolation.o: interpolation.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm *.o $(PROG)
