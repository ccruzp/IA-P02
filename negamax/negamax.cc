/*
 * Inteligencia Artificial - Proyecto 2
 * Negamax
 * Esteban Oliveros
 * Carlos Cruz
 * Cristian Medina
 */

#include "othello_cut.h" // won't work correctly until .h is fixed!
#include <iostream>
#include <climits>

#define MAXMOVE 32

using namespace std;

const int sign[2]={-1,1};   // 0 is white, 1 is black
int negamax(state_t node, int color) {
	if ( node.terminal() ) {
		//cout << "Found terminal node:\n";
		//cout << node << endl;
		//cout << "Whit value = " << sign[color] * node.value() << endl;
		return sign[color] * node.value();
	}

	//cout << "It's not terminal" << endl;
	int max = INT_MIN;
	vector<int> moves = node.get_succesors(color);
	//cout << "It has = " << moves.size() << " succesors" << endl;
	for(vector<int>::iterator it = moves.begin(); it != moves.end(); it++){
		state_t child = node.move(color, *it);
		int x = -negamax(child, 1-color);
		if ( x > max ) max = x;
	}
	
	return max;
}

int main(int argc, const char **argv) {

	state_t root;
	for(int d = MAXMOVE; d > 0; d--){
		state_t state(root);
		bool player;
		int pos;
		cout << "Move secuence = ";
		for( int i = 0; i <= d; ++i ) {
			player = i % 2 == 0; // black moves first!
			pos = PV[i];
			state = state.move(player, pos);
			cout << pos << " ";
		}
		cout << endl;
		//cout << state;
		//cout << "Next player = " << (1-player ? "Black" : "White") << endl;
		//cout << "Value of the game = " << state.value() << endl;
		//cout << "Last move = " << pos << endl;
		cout << "Value of negamax = " << negamax(state,1-player) << endl;	  
		cout << "----------------------------------------------" << endl;
	}
}

