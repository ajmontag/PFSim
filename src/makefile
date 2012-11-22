# Makefile for PFair Scheduler

CXX = g++-4
FLAGS = -Wall -g
COMPILEONLY = $(CXX) $(FLAGS) -c $<
LINK = $(CXX) $(FLAGS) -o $@ $+

# All targets to be made
.PHONY: all
all: pf


# Executable Targets
pf: main.o AlgoPF.o
	$(LINK)


# Object File Targets
main.o: main.cpp InputParameters.hpp Task.hpp AlgoPF.hpp
	$(COMPILEONLY)

AlgoPF.o: AlgoPF.cpp Schedule.hpp Task.hpp
	$(COMPILEONLY)

# Housekeeping targets
.PHONY: clean
clean:
	rm *.o

.PHONY: cleandist
cleandist:
	rm pf *.o
