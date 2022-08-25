
OBJS := $(subst .cpp,.o,$(wildcard *.cpp))

.PHONY:
all: $(OBJS)

-include $(subst .o,.d,$(OBJS))

%.o: %.d

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '(subst .d,.o,$@) $@' $< > $@

.PHONY:
clean:
	-rm *.o *.d
