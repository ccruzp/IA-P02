CC = gcc
CXX = g++
OPT = -g

all:
	make negamax
	make negamax_ab
	make scout
	make negascout

negamax :
	cp negamax/negamax.cc negamax.cc
	$(CXX) $(OPT) negamax.cc -include othello_cut.h -o negamax.p02
	rm negamax.cc

negamax_ab:
	
scout:

negascout:

clean:
	rm -f *.o *~ *.p02
