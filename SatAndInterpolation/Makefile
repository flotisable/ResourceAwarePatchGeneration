all: interpolation.o NtkToCnfConverter.o InterpolationEngine.o

interpolation.o: interpolation.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

NtkToCnfConverter.o: NtkToCnfConverter.cpp NtkToCnfConverter.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

InterpolationEngine.o: InterpolationEngine.cpp InterpolationEngine.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm *.o
