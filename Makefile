p1 : main.o
	g++ -Wall main.o -o p1

main.o : main_with_hash_table.cc  othello_cut.h
	g++ -Wall main_with_hash_table.cc -c othello_cut.h

clean:
	\rm *.o *~ p1
