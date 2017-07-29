all: test

test: TestInterpolationEngine.o $(satAndInterDir)/InterpolationEngine.o $(satAndInterDir)/NtkToCnfConverter.o $(abcLibDir)/$(abcLib)
	$(LD) $(LDFLAGS) -o $@ $^

TestInterpolationEngine.o: TestInterpolationEngine.cpp TestInterpolationEngine.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(satAndInterDir)/InterpolationEngine.o $(satAndInterDir)/NtkToCnfConverter.o:
	$(MAKE) -e -C $(projectDir)/$(satAndInterDir)

clean:
	rm test *.o
