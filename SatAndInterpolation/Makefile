all: interpolation.o NtkToCnfConverter.o

interpolation.o: interpolation.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

NtkToCnfConverter.o: NtkToCnfConverter.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm *.o
