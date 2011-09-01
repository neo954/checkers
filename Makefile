PROJECT = checkers
CHOST="x86_64-pc-linux-gnu"
CXXFLAGS += -std=c++98 -pedantic -Wall -Wextra -Winit-self -Winline -fno-common -pipe
#CXXFLAGS += -g -ggdb
#CXXFLAGS += -O0 -fno-inline
CXXFLAGS += -DNDEBUG
CXXFLAGS += -march=native
CXXFLAGS += -O3 --param max-inline-insns-single=9999 --param inline-unit-growth=9999
#CXXFLAGS += -fprofile-arcs -ftest-coverage
LINK.o = $(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH)

TARGETS = ponder runer

build: $(TARGETS)

ponder: absearch.o bitboard.o board.o engine.o evaluate.o io.o loopbuffer.o \
	move.o nonstdio.o record.o signal.o timeval.o zobrist.o

runer: io.o loopbuffer.o pipe.o signal.o

xcheckers: -lqt-mt

doc: Doxyfile *.cpp *.hpp
	mkdir -p doc
	doxygen

clean:
	$(RM) $(TARGETS) *.gcda *.gcno *.gcov *.o deps core gmon.out
	$(RM) -r doc

deps: *.cpp *.hpp
	$(CXX) -M $(CPPFLAGS) $^ >$@

include deps
.PHONY: build clean doc
