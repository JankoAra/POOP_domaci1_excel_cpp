#include "StateMachine.h"
#include "Errors.h"
/*
state==0	pocetak programa
state==1	aktivni rezim menjanja opcija tabele
state==2	ucitavanje tabele
*/
int StateMachine::state = 0;

//pair(currentState, selectedOption) ---> newState
map<pair<int, int>, int> StateMachine::stateChangeMap =\
{ {make_pair(0,1),1},{make_pair(0,2),2} };

void StateMachine::changeState(int choice){
	//if (choice == 0) return;
	pair<int, int> temp = make_pair(state, choice);
	if (!(state = stateChangeMap[temp])) {
		throw UnknownState();
	}
}
