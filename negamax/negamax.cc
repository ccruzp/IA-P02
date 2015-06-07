/*
 * Inteligencia Artificial - Proyecto 2
 * Negamax
 * Esteban Oliveros
 * Carlos Cruz
 * Cristian Medina
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

const int sign[2]={-1,1};   // 0 is white, 1 is black

int negamax(state_t node, int color, unsigned long long int &gen, unsigned long long int &eval ) {

	gen++;
	/* Si el nodo es terminal, calculamos el valor de ese estado del juego
	 * A diferencia del negamax original, no nos interesa la profundidad,
	 * evaluaremos todo el sub-arbol.
	 */
	if ( node.terminal() ) {
		eval++;
		return  sign[color] * node.value();
	}

	int max = INT_MIN;
	// Obtenemos los estado sucesores del presente juego (node)
	vector<int> moves = node.get_succesors(color);
	
	/* Si no existen movimientos, esto quiere decir que el jugador pierde su
	 * turno. Sin embargo, salir en este punto sería saltar un nivel del árbol
	 * e invertir los roles de los jugadores, cosa que no queremos.
	 * Por lo tanto, el valor para este juego y para el jugador/color actual
	 * será el mismo que este juego pero para el jugador/color contrario
	 */
	if (moves.empty()) return -negamax(node, 1-color, gen, eval);

	/* Por el contrario, si el juego y jugador/color actual si tiene movimientos
	 * procedemos a calcular de manera recursiva negamax para cada uno
	 * para obtener su valor, el cual es el max entre todos.
	 */
	for(vector<int>::iterator it = moves.begin(); it != moves.end(); it++){
		state_t child = node.move(color, *it);
		int x = -negamax(child, 1-color, gen, eval);
		if ( x > max ) max = x;
	}
	return max;
}

int main(int argc, const char **argv) {

	state_t root;
	cout << "Corrida de Negamax" << endl;
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
			// INIT
			clock_t t = clock(); 
			int nmax = sign[1-player] * negamax(state, 1-player, gen, eval);
			t = clock() - t;
			// END
			double seconds = (double) t / (double) CLOCKS_PER_SEC;	
			if (nmax != -4) {
				cout << "Resultado erroneo: " << nmax << endl;
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

