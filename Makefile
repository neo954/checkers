PROJECT = checkers
CXXFLAGS += -g -ggdb
CXXFLAGS += -std=c++98 -pedantic -Wall -Wextra -Winit-self -Winline
CXXFLAGS += -O9 --param max-inline-insns-single=900
LINK.o = $(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH)

TARGETS = ponder

build: $(TARGETS)

ponder: bitboard.o board.o engine.o intelligence.o io.o loopbuffer.o move.o \
	signal.o

doc: Doxyfile *.cpp *.hpp
	mkdir -p doc
	doxygen

clean:
	$(RM) $(TARGETS) *.o deps core gmon.out
	$(RM) -r doc

deps: *.cpp *.hpp
	$(CXX) -M $(CPPFLAGS) $^ >$@

include deps
.PHONY: build clean doc

