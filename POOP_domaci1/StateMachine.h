#ifndef _STATE_MACHINE_CLASS
#define _STATE_MACHINE_CLASS

#include <map>

using namespace std;

class StateMachine {
public:
	static int getState() { return state; }
	static void setState(int s) { state = s; }
	static void changeState(int choice);
private:
	static int state;
	static map<pair<int, int>, int> stateChangeMap;
};



#endif // !_STATE_MACHINE_CLASS
