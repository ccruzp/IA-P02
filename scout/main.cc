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

const int sign[2] = {-1, 1}; // pos 0 is white, pos 1 is black

bool testMayor(state_t state, int value, int player) {
  state_t child;
  if(state.terminal()) return (state.value() > value);

  vector<int> succesors = state.get_succesors(player);
  if(succesors.empty()) return !testMayor(state, value, 1-player);
  if(player == 1) {
    for(vector<int>::iterator it = succesors.begin(); it != succesors.end(); ++it) {
      child = state.move(true, *it);
      if(testMayor(child, value, 1-player)) return true;
    }
    return false;
  }
  if(player == 0) {
    for(vector<int>::iterator it = succesors.begin(); it != succesors.end(); ++it) {
      child = state.move(false, *it);
      if(!testMayor(child, value, 1-player)) return false;
    }
    return true;
  }
}

// bool test(state_t state, int value, int &acum, int player) {
bool testMenor(state_t state, int value, int player) {
  state_t child;
  if(state.terminal()) return (state.value() < value);

  vector<int> succesors = state.get_succesors(player);
  if(succesors.empty()) return !testMenor(state, value, 1-player);  

  if (player == 1) {
    for(vector<int>::iterator it = succesors.begin(); it != succesors.end(); ++it) {
      child = state.move(true, *it);
      if(!testMenor(child, value, 1-player)) return false;
    }
    return true;
  }
  
  if(player == 0) {
    for(vector<int>::iterator it = succesors.begin(); it != succesors.end(); ++it) {
      state.move(false, *it);
      if(testMenor(child, value, 1-player)) return true;
    }
    return false;
  }
}

int scout(state_t state, int player, unsigned long long int &gen, unsigned long long int &eval) {
  // cout << state;
  int value, acum = 0;
  state_t child;
  ++gen;
  if(state.terminal()) {
    ++eval;
    return sign[player] * state.value();
    // return state.value();

  // } else if(succesors.empty()) {
  //   cout << endl << "pase" << endl;
  //   // return -scout(state, 1-player, gen, eval);
  //   return -scout(state, 1-player, gen, eval);

  // } else {
  //   if(player) child = state.move(true, succesors.front());
  //   else child = state.move(false, succesors.front());
  //   value = -scout(child, 1-player, gen, eval);

  }
  vector<int> succesors = state.get_succesors(player);

  cout << "Jugador: " << player << endl;
  cout << "Vacio: " << succesors.empty() << endl;

  if(succesors.empty()) {

    cout << "Jugador " << player << " pasó" << endl;

    return -scout(state, 1-player, gen, eval);
  }
  child = state.move(player == 1, succesors.front());
  value = -scout(child, 1-player, gen, eval);
  if (player == 1) {

    cout << "player 1" << endl;

    for(vector<int>::iterator it = succesors.begin()+1; it != succesors.end(); ++it) {
      if (*it != succesors.front()) {
	child = state.move(true, *it);
	// if(!test(child, value, 1-player)) value += sign[1-player] * scout(child, 1-player, gen, eval);
	if(testMayor(child, value, 1-player)) value += -scout(child, 1-player, gen, eval);
      }
    }
  }
  if(player == 0) {
    for(vector<int>::iterator it = succesors.begin()+1; it != succesors.end(); ++it) {
      if (*it != succesors.front()) {
	child = state.move(false, *it);
	// if(test(child, value, 1-player)) value += sign[1-player] * scout(child, 1-player, gen, eval);
	if(testMenor(child, value, 1-player)) value += -scout(child, 1-player, gen, eval);

      }

    }

  }
  return value;
}

int main(int argc, const char **argv) {
  
  state_t root;
  cout << "Corrida de Scout" << endl;
  cout << "Estado | Tiempo | Evaluados | Generados" << endl;
  
  for(int d = MAXMOVE; d >= 0; d--){ // d >= -1 para que llegue a la raiz
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
      int nmax = sign[1-player] * scout(state, 1-player, gen, eval);
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
