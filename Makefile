CXXFLAGS += -O9
CXXFLAGS += -std=c++98 -pedantic -Wall -Wextra -Winit-self -Winline
CXXFLAGS += -g -ggdb
LINK.o = $(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH)

TARGETS = ponder
TESTS = test_loopbuffer

build: $(TARGETS)
test: $(TESTS)

ponder: bitboard.o board.o engine.o intelligence.o io.o loopbuffer.o move.o
test_loopbuffer: loopbuffer.o

doc: Doxyfile *.cpp *.hpp
	mkdir -p doc
	doxygen

clean:
	$(RM) $(TARGETS) $(TESTS) *.o deps core gmon.out
	$(RM) -r doc

deps: *.cpp *.hpp
	$(CXX) -M $(CPPFLAGS) $^ >$@

include deps
.PHONY: build clean doc

