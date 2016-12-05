CXX = g++
CXXFLAGS = -g -O2 -std=c++11 -Wall -MMD -static -static-libgcc -static-libstdc++ 

OBJECTS_CORE = main.o core/pieces.o core/board.o core/square.o core/move.o core/position.o core/movegen.o 
OBJECTS_TEST = test/test_main.o test/position_tests.o test/legal_move_tests.o
OBJECTS_AI = ai/ai_util.o ai/random_ai.o ai/minmax_ai.o ai/material_ai.o

EXEC = Engine

DEPENDS_CORE = ${OBJECTS_CORE:.o=.d}
DEPENDS_TEST = ${OBJECTS_TEST:.o=.d}
DEPENDS_AI = ${OBJECTS_AI:.o=.d}

EXECS = ${EXEC}

#############################################################

.PHONY : all clean

all : ${EXECS}

${EXEC} : ${OBJECTS_CORE} ${OBJECTS_TEST} ${OBJECTS_AI}
	${CXX} ${CXXFLAGS} $^ -o $@
  
#${EXEC} : ${OBJECTS_TEST}
#	${CXX} ${CXXFLAGS} $^ -o $@
  
#${EXEC} : ${OBJECTS_AI}
#	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS_CORE} : ${MAKEFILE_NAME}

-include ${DEPENDS_CORE}
-include ${DEPENDS_TEST}
-include ${DEPENDS_AI}

clean :
	rm -f *.d *.o core/*.d core/*.o test/*.d test/*.o ai/*.d ai/*.o ${EXECS}
