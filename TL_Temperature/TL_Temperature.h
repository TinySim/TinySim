#include <iostream>

using namespace std;

class TL_Temperature{
private:
	double current_temperature;
	int nodeId;
public:
	TL_Temperature();
	void read();
	double data();
	void setNodeId(int nodeID);
	int getNodeId();
};
