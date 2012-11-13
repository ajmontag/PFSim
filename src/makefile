# Makefile for PFair Scheduler

FLAGS = -Wall -g
COMPILEONLY = $(CXX) $(FLAGS) -c $<
LINK = $(CXX) $(FLAGS) -o $@ $+

# All targets to be made
.PHONY: all
all: pf


# Executable Targets
pf: main.o
	$(LINK)


# Object File Targets
main.o: main.cpp InputParameters.hpp
	$(COMPILEONLY)


# Housekeeping targets
.PHONY: clean
clean:
	rm *.o

.PHONY: cleandist
cleandist:
	rm pf *.o
