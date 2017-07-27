abcDir    := $(HOME)/abc
abcSrcDir := $(abcDir)/src
abcLibDir := $(abcDir)
abcLib    := libabc.a
abcFlags  := $(shell ./getAbcFlags.sh $(abcDir))

PROG      := rpgen
CXX       := g++ 
LD        := g++
INC       := -I$(abcSrcDir)
CXXFLAGS  := $(INC) $(abcFlags) -g -O0
LDFLAGS   := -lreadline -lpthread -ldl -lm
 
all: $(PROG)

$(PROG): pre_process.o read_file.o delete_unused_PO.o trav_Po_add_to_set.o read_weight.o main.o ResourceAwarePatchGeneration.o write_patch.o interpolation.o $(abcLibDir)/$(abcLib) 
	$(LD) $(LDFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
	
ResourceAwarePatchGeneration.o: ResourceAwarePatchGeneration.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

pre_process.o: pre_process.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
	
read_file.o: read_file.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

delete_unused_PO.o: delete_unused_PO.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

trav_Po_add_to_set.o: trav_Po_add_to_set.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
	
read_weight.o: read_weight.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

write_patch.o: write_patch.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

interpolation.o: interpolation.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm *.o $(PROG)
