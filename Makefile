PROJECT = checkers
#CXXFLAGS += -DNDEBUG
CXXFLAGS += -g -ggdb
CXXFLAGS += -std=c++98 -pedantic -Wall -Wextra -Winit-self -Winline
CXXFLAGS += -O9 --param max-inline-insns-single=9999
LINK.o = $(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH)

TARGETS = ponder runer

build: $(TARGETS)

ponder: bitboard.o board.o engine.o intelligence.o io.o loopbuffer.o move.o \
	signal.o zobrist.o

runer: io.o loopbuffer.o pipe.o signal.o

xcheckers: -lqt-mt

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

