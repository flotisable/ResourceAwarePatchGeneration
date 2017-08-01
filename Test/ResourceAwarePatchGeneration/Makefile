all: test

test: main.o TestResourceAwarePatchGeneration.o \
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
	     $(satAndInterDir)/interpolation.o \
	     $(satAndInterDir)/NtkToCnfConverter.o \
			 $(satAndInterDir)/InterpolationEngine.o \
	     $(abcLibDir)/$(abcLib)
	$(LD) $(LDFLAGS) -o $@ $^

main.o: main.cpp TestResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

TestResourceAwarePatchGeneration.o: TestResourceAwarePatchGeneration.cpp TestResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

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
$(satAndInterDir)/interpolation.o \
$(satAndInterDir)/NtkToCnfConverter.o \
$(satAndInterDir)/InterpolationEngine.o:
	$(MAKE) -C $(projectDir)

clean:
	rm test *.o
