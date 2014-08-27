RM=rm -f
CXX=g++
LD=ld
CFLAGS=-c -Wall -MMD
LDFLAGS=-lprofiler
HW1_MAIN=hw1.cpp
HW1_OBJECTS=$(HW1_MAIN:.cpp=.o)
HW1_DEPS=$(HW1_OBJECTS:.o=.d)
HW1_EXE=parser.bin
HW1_PROF=parser_prof.bin

MKI_MAIN=mkinput.cpp 
MKI_OBJECTS=$(HW1_MAIN:.cpp=.o)
MKI_DEPS=$(HW1_OBJECTS:.o=.d)
MKI_EXE=mkinput.bin 

all: $(HW1_MAIN) $(HW1_EXE)

$(HW1_EXE): $(HW1_OBJECTS)
	$(CXX) $(HW1_OBJECTS) -o $@
-include $(HW1_DEPS)

input: $(MKI_MAIN) $(MKI_EXE)

$(MKI_EXE): $(MKI_OBJECTS)
	$(CXX) $(MKI_OBJECTS) -o $@
-include $(MKI_DEPS)

profile: $(HW1_MAIN) $(HW1_PROF)

$(HW1_PROF): $(HW1_OBJECTS)
	$(CXX) $(LDFLAGS) $(HW1_OBJECTS) -o $@
-include $(HW1_DEPS)

.cpp.o:
	$(CXX) $(CFLAGS) -MF $(patsubst %.o,%.d,$@) $< -o $@

clean:
	$(RM) $(MKI_OBJECTS) $(MKI_DEPS) $(MKI_EXE) $(MKI_OBJECTS) $(MKI_DEPS) $(MKI_EXE)
