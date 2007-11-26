PROJECT = checkers
CXXFLAGS += -std=c++98 -pedantic -Wall -Wextra -Winit-self -Winline
CXXFLAGS += -g -ggdb
CXXFLAGS += -O0 -fno-inline
#CXXFLAGS += -DNDEBUG
#CXXFLAGS += -O3 --param max-inline-insns-single=9999
LINK.o = $(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH)

TARGETS = ponder runer

build: $(TARGETS)

ponder: bitboard.o board.o engine.o evaluate.o intelligence.o io.o \
	loopbuffer.o move.o record.o signal.o timeval.o zobrist.o

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

