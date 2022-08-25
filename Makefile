# project directories
projectDir     := $(PWD)
satAndInterDir := $(projectDir)/SatAndInterpolation
testDir        := $(projectDir)/Test
# end project directories

# abc related variables
$(info Collecting abc flags)
abcDir    := $(projectDir)/Lib/abc
abcSrcDir := $(abcDir)/src
abcLibDir := $(abcDir)
abcLib    := libabc.a
abcFlags  := $(shell ./getAbcFlags.sh $(abcDir))
# end abc related variables

# project related variables
PROG      := rpgen
CXX       := g++ 
LD        := g++
INC       := -I$(projectDir) -I$(abcSrcDir)
CXXFLAGS  := $(INC) $(abcFlags)
LDFLAGS   := -lreadline -lpthread -ldl -lm
# end project related variables

# project object files
projectLinkedFiles     := $(subst .cpp,.o,$(filter-out $(projectDir)/main.cpp,$(wildcard $(projectDir)/*.cpp)))
satAndInterLinkedFiles := $(subst .cpp,.o,$(wildcard $(satAndInterDir)/*.cpp))

linkedFiles := \
	$(projectLinkedFiles) \
	$(satAndInterLinkedFiles) \
  $(abcLibDir)/$(abcLib) 
# end project object files

export

.PHONY: all
all: $(PROG)

# main targets
$(PROG): main.o $(linkedFiles)
	$(LD) $(LDFLAGS) -o $@ $^

-include $(subst .o,.d,$(projectLinkedFiles))
-include $(subst .o,.d,$(satAndInterLinkedFiles))

%.o: %.d

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '(subst .d,.o,$@) $@' $< > $@

$(satAndInterLinkedFiles): $(satAndInterDir)/interpolation.cpp $(satAndInterDir)/NtkToCnfConverter.cpp $(satAndInterDir)/InterpolationEngine.cpp $(satAndInterDir)/NtkToCnfConverter.h $(satAndInterDir)/InterpolationEngine.h
	$(MAKE) -e -C $(satAndInterDir)
# end main targets

.PHONY: debug
debug: CXXFLAGS += -g -O
debug: $(PROG)

$(abcLibDir)/$(abcLib):
	$(MAKE) -C $(abcDir) $(abcLib)

# targets for test
testNtkToCnfConverter:
	$(MAKE) -e -C $(testDir)/NtkToCnfConverter

testInterpolationEngine:
	$(MAKE) -e -C $(testDir)/InterpolationEngine

testResourceAwarePatchGeneration:
	$(MAKE) -e -C $(testDir)/ResourceAwarePatchGeneration
# end targets for test

.PHONY: clean
clean:
	-rm *.o *.d $(PROG)
	-$(MAKE) -C $(satAndInterDir) clean
	-$(MAKE) -C $(abcDir) clean
