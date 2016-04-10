CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -MMD

OBJECTS = main.o board.o position.o movegen.o
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
	rm -f *.d *.o ${EXECS}
