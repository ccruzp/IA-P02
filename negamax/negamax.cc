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

const int sign[2]={1,-1}   // 0 is black, 1 is white

using namespace std;

int main(int argc, const char **argv) {

	state_t state;
	
/*
  state_t state;
  cout << "Principal variation:" << endl;
  for( int i = 0; PV[i] != -1; ++i ) {
		bool player = i % 2 == 0; // black moves first!
		int pos = PV[i];
		cout << state;
		cout << (player ? "Black" : "White")
			<< " moves at pos = " << pos << (pos == 36 ? " (pass)" : "")
			<< endl;
		state = state.move(player, pos);
		cout << "Board after " << i+1 << (i == 0 ? " ply:" : " plies:") << endl;
  }
  cout << state;
  cout << "Value of the game = " << state.value() << endl;
  cout << "#bits per state = " << sizeof(state) * 8 << endl;

  return 0;
*/
}

int negamax(state_t node, int color) {
	if ( node.terminal() )
		return sign[color] * node.value();
    
  max = INT_MIN;
  vector<int> moves = node.get_succesor(color);
  
  for(vector<int>::iterator moves = node.begin(); moves != node.end(); moves++){
  	state_t child = node.move(color, *moves);
  	int x = -negamax(child, 1-color);
  	if ( x > max ) max = x;
  }
}
