/*
 * Inteligencia Artificial - Proyecto 2
 * Scout
 * Carlos Cruz
 * Cristian Medina
 * Esteban Oliveros
 */

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

#define MAXMOVE 32
#define TIMEOUT 1200

using namespace std;

void espacios(int d){
	for(int i=0; i < d; i++)
		cout << "    ";
}

int test(int d, state_t node, int color, int value, int signo){ 
	
	if ( node.terminal() ) {
		//espacios(d);
		//cout << "*T" << d << (color? "B:":"W:") << node.value() << ">" << value << " " << (node.value() > value) << endl;
		return node.value() > value;
	}
	
	vector<int> moves = node.get_succesors(color);
	
	if (moves.empty()) {
		int val = test(d+1, node, 1-color, value, signo);
		//espacios(d);
		//cout << "*JS" << d <<(color? "B:":"W:") << val << endl;
		return val;
	}

	for(vector<int>::iterator it = moves.begin(); it != moves.end(); it++){
		state_t child = node.move(color, *it);
		
		// If node is MAX and TEST=1
		if (color && test(d+1, child, 1-color, value, signo)) {
			//espacios(d);
			//cout << "*>S" << d << (color? "B:1":"W:1") << endl;
			return 1;
		// If node is MIN and TEST=0
		} else if (!color && !test(d+1, child, 1-color, value, signo)) {
			//espacios(d);
			//cout << "*<S" << d << (color? "B:0":"W:0") << endl;
			return 0;
		}
	}
	//espacios(d);
	//cout << "*?S" << d << (color? "B:":"W:") << (color? 0:1) << endl;
	return (color? 0:1);
}

int scout(int d, state_t node, int color, unsigned long long int &gen, unsigned long long int &eval){

	gen++;

	if ( node.terminal() ) {
		eval++;
		//espacios(d);
		//cout << "T" << d << (color? "B:":"W:") << node.value() << endl;
		return  node.value();
	}
	
	int score = 0;
	// Obtenemos los estado sucesores del presente juego (node)
	vector<int> moves = node.get_succesors(color);
	
	if (moves.empty()) {
		int val = scout(d+1, node, 1-color, gen, eval);
		//espacios(d);
		//cout << "JS" << d <<(color? "B:":"W:") << val << endl;
		return val;
	}

	int first = 1;
	for(vector<int>::iterator it = moves.begin(); it != moves.end(); it++){
		state_t child = node.move(color, *it);
		
		if ( first ) {
			score = scout(d+1, child, 1-color, gen, eval);
			//espacios(d+1);
			//cout << "=" << endl;
			first = 0;
		} else {
			// If node is MAX and TEST=1
			if (color && test(d+1, child, 1-color, score, 1)) {
				score = scout(d+1, child, 1-color, gen, eval);
				//espacios(d+1);
				//cout << "< " << color << " " << score << endl;
			// If node is MIN and TEST=0
			} else if (!color && !test(d+1, child, 1-color, score, 0)) {
				score = scout(d+1, child, 1-color, gen, eval);
				//espacios(d+1);
				//cout << "> " << 1-color << " " << score << endl;
			}
		}
	}
	//espacios(d);
	//cout << "S" << d << (color? "B:":"W:") << score << endl;
	return score;
}

int main(int argc, const char **argv) {
  
  state_t root;
  cout << "Corrida de Scout" << endl;
  cout << "Estado | Tiempo | Evaluados | Generados" << endl;
  
  for(int d = MAXMOVE; d >= 0; d--){ // d >= -1 para que llegue a la raiz
  	//if (d!=28) continue;
    state_t state(root);
    bool player;
    int pos;

    for( int i = 0; i <= d; ++i ) {
      player = i % 2 == 0; // black moves first!
      pos = PV[i];			
      state = state.move(player, pos);
      // cout << state << endl;

    }
    
    pid_t pid = fork();
    
    if (pid == -1) {
      perror("fork failed");
      exit(1);

    } else if (pid == 0) {
      unsigned long long int eval = 0, gen = 0;
      // INIT
      clock_t t = clock(); 
      int nmax = scout(0, state, 1-player, gen, eval);
      t = clock() - t;
      // END
      double seconds = (double) t / (double) CLOCKS_PER_SEC;	
      if (nmax != -4) {
				cout << "Resultado erroneo: " << nmax << endl;
      }
      cout << d << " | " << seconds << " | " << eval << " | " << gen << endl;
      exit(0);

    } else {
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
