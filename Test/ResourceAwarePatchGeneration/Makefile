all: test

test: main.o TestResourceAwarePatchGeneration.o $(linkedFiles)
	$(LD) $(LDFLAGS) -o $@ $^

main.o: main.cpp TestResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

TestResourceAwarePatchGeneration.o: TestResourceAwarePatchGeneration.cpp TestResourceAwarePatchGeneration.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(linkedFiles):
	$(MAKE) -C $(projectDir)

clean:
	rm test *.o
