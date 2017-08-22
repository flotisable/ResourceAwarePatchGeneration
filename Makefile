# abc related variables
abcDir    := $(HOME)/abc
abcSrcDir := $(abcDir)/src
abcLibDir := $(abcDir)
abcLib    := libabc.a
abcFlags  := $(shell ./getAbcFlags.sh $(abcDir))
# end abc related variables

# project directories
projectDir			:= $(PWD)
satAndInterDir 	:= $(projectDir)/SatAndInterpolation
testDir					:= $(projectDir)/Test
# end project directories

# project related variables
PROG      := rpgen
CXX       := g++ 
LD        := g++
INC       := -I$(projectDir) -I$(abcSrcDir)
CXXFLAGS  := $(INC) $(abcFlags)
LDFLAGS   := -lreadline -lpthread -ldl -lm
# end project related variables

# project object files
projectLinkedFiles := \
  $(projectDir)/pre_process.o \
  $(projectDir)/traverse_t_PI_and_PO.o \
  $(projectDir)/trav_Pi_add_to_set.o \
  $(projectDir)/replace_t_with_PI.o \
  $(projectDir)/construct_t.o \
  $(projectDir)/read_file.o \
  $(projectDir)/delete_unused_PO.o \
  $(projectDir)/trav_Po_add_to_set.o \
  $(projectDir)/read_weight.o \
  $(projectDir)/ResourceAwarePatchGeneration.o \
  $(projectDir)/write_patch.o \
  $(projectDir)/functional_dependency.o \
  $(projectDir)/sat_solving.o

satAndInterLinkedFiles := \
  $(satAndInterDir)/interpolation.o \
  $(satAndInterDir)/NtkToCnfConverter.o \
  $(satAndInterDir)/InterpolationEngine.o

linkedFiles := \
	$(projectLinkedFiles) \
	$(satAndInterLinkedFiles) \
  $(abcLibDir)/$(abcLib) 
# end project object files

export

all: $(PROG)

# main targets
$(PROG): main.o $(linkedFiles)
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

sat_solving.o: sat_solving.cpp $(satAndInterDir)/NtkToCnfConverter.h ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

write_patch.o: write_patch.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

functional_dependency.o: functional_dependency.cpp ResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

$(satAndInterLinkedFiles): $(satAndInterDir)/interpolation.cpp $(satAndInterDir)/NtkToCnfConverter.cpp $(satAndInterDir)/InterpolationEngine.cpp $(satAndInterDir)/NtkToCnfConverter.h $(satAndInterDir)/InterpolationEngine.h
	$(MAKE) -e -C $(satAndInterDir)
# end main targets

debug: CXXFLAGS += -g -O
debug: all

# targets for test
testNtkToCnfConverter:
	$(MAKE) -e -C $(testDir)/NtkToCnfConverter

testInterpolationEngine:
	$(MAKE) -e -C $(testDir)/InterpolationEngine

testResourceAwarePatchGeneration:
	$(MAKE) -e -C $(testDir)/ResourceAwarePatchGeneration
# end targets for test

clean:
	rm *.o $(PROG)
	$(MAKE) -C $(satAndInterDir) clean
