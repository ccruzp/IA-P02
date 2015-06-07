#include <vector>
#include "othello_cut.h"
#include <iostream>
#include <climits>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <ctime>
#include <algorithm>  

#define MAXMOVE 32
#define TIMEOUT 1200
using namespace std;

const int sign[2]={-1,1};   // 0 is white, 1 is black

int negascout(state_t node, int color, int alpha, int beta, unsigned long long int &gen, unsigned long long int &eval){
	
	gen++;	
  
	if (node.terminal()){
		eval++;		
		return  sign[color] * node.value();
	}

	int a = INT_MIN+1;
	int b = beta;

  state_t child;
  vector<int> succesors = node.get_succesors(color);

	if (succesors.empty()) return -negascout(node, 1-color, -beta, -alpha, gen, eval);

	for (vector<int>::iterator it = succesors.begin(); it != succesors.end(); it++) {

		state_t child = node.move(color, *it);

		int t = -negascout(child, 1-color, -b, -(max(a,alpha)), gen, eval);
		if (t > a) {
			if (b == beta || t >= beta) {
				a = t;		
			} else {
				a = -negascout(child, 1-color, -beta, -t, gen, eval);
			}
			if (a >= beta) {
				eval++;
				return a;
			}
			b = max(alpha, a) + 1;
		}
		
	}
	eval++;
	return a;

}




int main(int argc, const char **argv) {

	state_t root;
	cout << "Corrida de Negascout" << endl;
	cout << "Estado | Tiempo | Evaluados | Generados" << endl;
	
	for(int d = MAXMOVE; d >= 0; d--){ // d >= -1 para que llegue a la raiz
		state_t state(root);
		bool player;
		int pos;
		for( int i = 0; i <= d; ++i ) {
			player = i % 2 == 0; // black moves first!
			pos = PV[i];			
			state = state.move(player, pos);
		}

		pid_t pid = fork();

		if (pid == -1) {
			perror("fork failed");
			exit(1);
		}
		else if (pid == 0) {
			unsigned long long int eval = 0, gen = 0;
			clock_t t = clock(); // INIT
			int nmax = sign[1-player] * negascout(state, 1-player,INT_MIN+1, INT_MAX-1, gen, eval);
			t = clock() - t; // END
			
			double seconds = (double) t / (double) CLOCKS_PER_SEC;	
			if (nmax != -4) {
				cout << "Resultado erroneo: " << nmax << endl;
				exit(1);
			}
			cout << d << " | " << seconds << " | " << eval << " | " << gen << endl;
			exit(0);
		}
		else {
			int waittime = 0;
			int status;
			pid_t wpid;
			while ( waittime <= TIMEOUT ) {
				wpid = waitpid(pid, &status, WNOHANG);
				if (wpid == 0){
					waittime ++;
					sleep(1);
				} else {
					break;
				}
			}
			
			if ( waittime > TIMEOUT ) {					
				kill(pid, SIGKILL); 
				cout << "Se acabo el tiempo" << endl;
				exit(1);
			}
			
		}		
	}
	return 0;
}
