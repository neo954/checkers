CXXFLAGS += -g -ggdb -Wall
LINK.o = $(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH)

TARGETS = ponder
TESTS = test_loopbuffer

build: $(TARGETS)
test: $(TESTS)

ponder: engine.o bitboard.o board.o move.o loopbuffer.o io.o
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

