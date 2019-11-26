#include <iostream>

using namespace std;

class TL_Light{
private:
	double current_intensity;
	int nodeId;
public:
	TL_Light();
	void read();
	void setData(double intensity);
	double data();
	void setNodeId(int nodeId);
	int getNodeId();
};
