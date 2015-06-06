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

#define MAXMOVE 32

using namespace std;

const int sign[2]={-1,1};   // 0 is white, 1 is black
int negamax(state_t node, int color) {

	/* Si el nodo es terminal, calculamos el valor de ese estado del juego
	 * A diferencia del negamax original, no nos interesa la profundidad,
	 * evaluaremos todo el sub-arbol.
	 */
	if ( node.terminal() ) 	return sign[color] * node.value();

	int max = INT_MIN;
	// Obtenemos los estado sucesores del presente juego (node)
	vector<int> moves = node.get_succesors(color);
	
	/* Si no existen movimientos, esto quiere decir que el jugador pierde su
	 * turno. Sin embargo, salir en este punto sería saltar un nivel del árbol
	 * e invertir los roles de los jugadores, cosa que no queremos.
	 * Por lo tanto, el valor para este juego y para el jugador/color actual
	 * será el mismo que este juego pero para el jugador/color contrario
	 */
	if (moves.empty()) return -negamax(node, 1-color);

	/* Por el contrario, si el juego y jugador/color actual si tiene movimientos
	 * procedemos a calcular de manera recursiva negamax para cada uno
	 * para obtener su valor, el cual es el max entre todos.
	 */
	for(vector<int>::iterator it = moves.begin(); it != moves.end(); it++){
		state_t child = node.move(color, *it);
		int x = -negamax(child, 1-color);
		if ( x > max ) max = x;		
	}
	
	return max;
}

int main(int argc, const char **argv) {

	state_t root;
		
	for(int d = MAXMOVE; d >= -1; d--){ // d >= -1 para que llegue a la raiz
		cout << "----------------------------------------------" << endl;
		state_t state(root);
		bool player;
		int pos;
		for( int i = 0; i <= d; ++i ) {
			player = i % 2 == 0; // black moves first!
			pos = PV[i];			
			state = state.move(player, pos);
		}
		cout << state;
		// Negamax debe ser siempre -4
		int nmax = sign[1-player]*negamax(state,1-player);
		cout << "Value of negamax = " << nmax << endl;		
		assert(nmax == -4);
	}
}

