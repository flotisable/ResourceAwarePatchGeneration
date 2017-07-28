all: test

test: main.o TestNtkToCnfConverter.o
	$(LD) $(LDFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

TestNtkToCnfConverter.o: TestNtkToCnfConverter.cpp TestNtkToCnfConverter.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm test *.o
