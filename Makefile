CXXFLAGS += -g -ggdb -Wall
LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

TARGET = main
OBJECTS = main.o bitboard.o board.o move.o interface.o console.o

build: $(TARGET)

$(TARGET): $(OBJECTS)

doc: Doxyfile *.cpp *.hpp
	mkdir -p doc
	doxygen

clean:
	$(RM) $(OBJECTS) $(TARGET) deps core gmon.out
	$(RM) -r doc

deps: *.cpp *.hpp
	$(CXX) -M $(CPPFLAGS) $^ >$@

include deps
.PHONY: build clean doc deps

