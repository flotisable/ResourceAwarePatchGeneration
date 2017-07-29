all: test

test: main.o TestNtkToCnfConverter.o $(satAndInterDir)/NtkToCnfConverter.o $(abcLibDir)/$(abcLib)
	$(LD) $(LDFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

TestNtkToCnfConverter.o: TestNtkToCnfConverter.cpp TestNtkToCnfConverter.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(satAndInterDir)/NtkToCnfConverter.o:
	$(MAKE) -e -C $(satAndInterDir)

clean:
	rm test *.o
