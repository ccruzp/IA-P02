/*
*  Copyright (C) 2012 Universidad Simon Bolivar
 * 
 *  Permission is hereby granted to distribute this software for
 *  non-commercial research purposes, provided that this copyright
 *  notice is included with any such distribution.
 *  
  *  THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
  *  EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
  *  SOFTWARE IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
  *  ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 *  
  *  Blai Bonet, bonet@ldc.usb.ve
 *
  *  Last revision: 11/08/2012
 *
 */

#include "othello_cut.h" 
#include <iostream>

using namespace std;

bool test(state_t state, int value, int player) {
  vector<int> succesors = state.get_succesors(player);
  if(succesors.empty()) return(state.value() < value);
  
  if (player) {
    for(vector<int>::iterator it = succesors.begin(); it != succesors.end(); ++it) {
      state_t child = state.move(1-player, *it);
      if(!test(child, value, 1-player)) return false;
    }
    return true;
  }
  
  if(!player) {
    for(vector<int>::iterator it = succesors.begin(); it != succesors.end(); ++it) {
      state_t child = state.move(1-player, *it);
      if(test(child, value, 1-player)) return true;
    }
    return false;
  }
}

int scout(state_t state, int player) {
  int value;
  state_t child;
  vector<int> succesors = state.get_succesors(player);
  if(succesors.empty()) {
    return state.value();
  } else {
    child = state.move(1-player, succesors.at(0));
    value = scout(child, 1-player);
  }
  if (player) {
    for(vector<int>::iterator it = succesors.begin()+1; it != succesors.end(); ++it) {
      child = state.move(1-player, *it);
      if(!test(child, value, 1-player)) value = scout(child, 1-player);
    }
  }
  if(!player) {
    for(vector<int>::iterator it = succesors.begin()+1; it != succesors.end(); ++it) {
      child = state.move(1-player, *it);
      if(test(child, value, 1-player)) value = scout(child, 1-player);
    }
  }
  return value;
}

int main(int argc, const char **argv) {
  state_t state;
  cout << state;
  vector<int> s = state.get_succesors(0);
  for(vector<int>::iterator it = s.begin(); it != s.end(); ++it) {
    state_t estado = state.move(0, *it);
    cout << estado;
  }
  
  // cout << "Principal variation:" << endl;
  // for( int i = 0; PV[i] != -1; ++i ) {
  //   bool player = i % 2 == 0; // black moves first!
  //   int pos = PV[i];
  //   cout << state;
  //   cout << (player ? "Black" : "White")
  // 	 << " moves at pos = " << pos << (pos == 36 ? " (pass)" : "")
  // 	 << endl;
  //   state = state.move(player, pos);
  //   cout << "Board after " << i+1 << (i == 0 ? " ply:" : " plies:") << endl;
  // }
  // cout << state;
  // cout << "Value of the game = " << state.value() << endl;
  // cout << "#bits per state = " << sizeof(state) * 8 << endl;

  // if( argc > 1 ) {
  //   int n = atoi(argv[1]);
  //   cout << endl << "Apply " << n << " random movements at empty board:";
  //   state = state_t();
  //   for( int i = 0; i < n; ++i ) {
  //     bool player = i % 2 == 0; // black moves first
  //     int pos = state.get_random_move(player);
  //     state = state.move(player, pos);
  //     cout << " " << pos;
  //   }
  //   cout << endl << state;
  // }

  return 0;
}

