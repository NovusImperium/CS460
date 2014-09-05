RM=rm -rf
MKDIR=mkdir
TAR=tar czf
CXX=g++
CFLAGS=-c -Wall -std=c++11 -MMD
DEBUG_FLAGS=-g -lprofiler

HWNUM = H02
COMMON_SOURCES = gcd.cpp Fraction.cpp
MAIN = gill$(HWNUM).cpp
TARGET_SOURCES = $(MAIN)
TEST_SOURCES = mkinput.cpp
COMMON_OBJECTS = $(COMMON_SOURCES:.cpp=.o)
TARGET_OBJECTS = $(TARGET_SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)
COMMON_DEPS = $(COMMON_OBJECTS:.o=.d)
TARGET_DEPS = $(TARGET_OBJECTS:.o=.d)
TEST_DEPS = $(TEST_OBJECTS:.o=.d)
EXECUTABLE = $(HWNUM).out
TEST_EXECUTABLE = $(TEST_SOURCES:.cpp=.bin)

FILE = $(TARGET_SOURCES:.cpp=/)
TARBALL = $(TARGET_SOURCES:.cpp=.tgz)
DROPBOX = ~tiawatts/cs460drop/

INPUT = input
OUTPUT = output

.PHONY: all target tests run tarball turnin

all: target 

target: $(COMMON_SOURCES) $(TARGET_SOURCES) $(EXECUTABLE)

tests: $(TEST_SOURCES) $(TEST_EXECUTABLE)

debug: $(DEBUG_EXE)

$(EXECUTABLE): $(COMMON_OBJECTS) $(TARGET_OBJECTS)
	$(CXX) $^ -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CXX) $^ -o $@

$(DEBUG_EXE): $(COMMON_OBJECTS) $(TARGET_OBJECTS)
	$(CXX) $(DEBUG_FLAGS) $^ -o $@

-include $(COMMON_DEPS) $(TARGET_DEPS) $(TEST_DEPS)

.cpp.o:
	$(CXX) $(CFLAGS) -MF $(patsubst %.o,%.d,$@) $< -o $@

run: all
	./$(EXECUTABLE) < $(INPUT) > $(OUTPUT)

input: tests
	./$(TEST_EXECUTABLE) > $(INPUT)

tarball:
	$(MKDIR) $(FILE)
	cp -t $(FILE) Makefile $(TARGET_SOURCES) $(COMMON_SOURCES)
	$(TAR) $(TARBALL) $(FILE)
	$(RM) $(FILE)

turnin: tarball 
	mv $(TARBALL) $(DROPBOX)

clean:
	$(RM) $(COMMON_OBJECTS) $(COMMON_DEPS) $(TARGET_DEPS) $(TARGET_OBJECTS) $(TEST_DEPS) $(TEST_OBJECTS) \
		$(EXECUTABLE) $(TEST_EXECUTABLE) $(DEBUG_EXE) $(INPUT) $(OUTPUT)
