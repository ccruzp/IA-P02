CC = gcc
CXX = g++
OPT = -g

all:
	make main
	make with_hash

main : othello_cut.h
	$(CXX) $(OPT)  main.cc -o main

with_hash : othello_cut.h
	$(CXX) $(OPT)  main_with_hash_table.cc -o with_hash

clean:
	rm *.o *~ main with_hash
