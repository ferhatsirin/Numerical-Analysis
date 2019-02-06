CC =gcc
CFLAGS = -Wall -pedantic-errors
LIBS = -lm

all : main

main : compile
	gcc hw2_solver.o  $(LIBS) -o solver

compile : 
	gcc -c hw2_solver.c $(CFLAGS) 

clear :
	rm hw2_solver.o solver
