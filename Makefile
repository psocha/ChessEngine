CXX=g++
RM=rm -rf
CXXFLAGS=-g -O2 -std=c++11 -Wall -MMD -static -static-libgcc -static-libstdc++
LDFLAGS=$(CXXFLAGS)

SRCS=main.cc ai/ai_util.cc ai/material_ai.cc ai/material_positional_ai.cc ai/minmax_ai.cc ai/random_ai.cc core/board.cc core/move.cc core/movegen.cc core/pieces.cc core/position.cc core/square.cc test/ai_tests.cc test/legal_move_tests.cc test/move_stack_tests.cc test/position_tests.cc test/performance_tests.cc test/test_main.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: Engine

Engine: $(OBJS)
	$(CXX) $(LDFLAGS) -o Engine $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS) *.d ai/*.d core/*.d test/*.d
