#include <iostream>
#include <cstring>

using namespace std;

class TL_LED{
private:
	string current_state;
	int nodeId;

public:
	TL_LED();
	~TL_LED(){}
	void toggle();
	void turnOn();
	void turnOff();
	string state();
	void setNodeId(int node);
};
