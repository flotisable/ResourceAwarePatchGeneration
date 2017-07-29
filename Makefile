abcDir    := $(HOME)/abc
abcSrcDir := $(abcDir)/src
abcLibDir := $(abcDir)
abcLib    := libabc.a
abcFlags  := $(shell ./getAbcFlags.sh $(abcDir))

projectDir			:= $(PWD)
satAndInterDir 	:= SatAndInterpolation
testDir					:= Test

PROG      := rpgen
CXX       := g++ 
LD        := g++
INC       := -I$(projectDir) -I$(abcSrcDir)
CXXFLAGS  := $(INC) $(abcFlags)
LDFLAGS   := -lreadline -lpthread -ldl -lm

export

all: $(PROG)

$(PROG): pre_process.o \
	     trav_Pi_add_to_set.o \
	     replace_t_with_PI.o \
	     construct_t.o \
	     traverse_t_PI_and_PO.o \
	     read_file.o \
	     delete_unused_PO.o \
	     trav_Po_add_to_set.o \
	     read_weight.o main.o \
	     ResourceAwarePatchGeneration.o \
	     write_patch.o \
	     $(satAndInterDir)/interpolation.o \
	     $(satAndInterDir)/NtkToCnfConverter.o \
	     $(abcLibDir)/$(abcLib) 
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

trav_Pi_add_to_set.o: trav_Pi_add_to_set.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

traverse_t_PI_and_PO.o: traverse_t_PI_and_PO.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

replace_t_with_PI.o: replace_t_with_PI.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
	
read_weight.o: read_weight.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

construct_t.o: construct_t.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

write_patch.o: write_patch.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

$(satAndInterDir)/interpolation.o $(satAndInterDir)/NtkToCnfConverter.o:
	$(MAKE) -e -C $(satAndInterDir)

debug: CXXFLAGS += -g -O
debug: all

testNtkToCnfConverter:
	$(MAKE) -e -C $(testDir)/TestNtkToCnfConverter

clean:
	rm *.o $(PROG)
	$(MAKE) -C $(satAndInterDir) clean
	$(MAKE) -C $(testDir)/TestNtkToCnfConverter clean
