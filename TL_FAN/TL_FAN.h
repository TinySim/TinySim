#include <iostream>
#include <string>

using namespace std;

class TL_FAN{
private:
	string current_state;
	int nodeId;
public:
	TL_FAN();
	void turnOn();
	void turnOff();
	string state();
	void toggle();
	void setNodeId(int nodeID);
	int getNodeId();
};
