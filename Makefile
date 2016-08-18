CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -MMD

OBJECTS = main.o test/integration_tests.o core/board.o core/coordinates.o core/position.o core/movegen.o
EXEC = random.exe
DEPENDS = ${OBJECTS:.o=.d}
EXECS = ${EXEC}

#############################################################

.PHONY : all clean

all : ${EXECS}								# build all executables

${EXEC} : ${OBJECTS}							# link step
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}						# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}							# include *.d files containing program dependences

clean :									# remove files that can be regenerated
	rm -f *.d *.o core/*.d core/*.o test/*.d test/*.o ${EXECS}
