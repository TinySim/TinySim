#include <iostream>

using namespace std;

class TL_Soil_Humidity{
private:
	double current_soil_humidity;
	int nodeId;
public:
	TL_Soil_Humidity();
	void read();
	double data();
	void setNodeId(int nodeId);
	int getNodeId();
};
