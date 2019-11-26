#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class TL_SIM_mode{
public:
	static void setMode(int real_or_virtual);
	static void addOutput(int state);
	static void outputPeriod(int interval); // ms
	static void start();  // make mode effective
	static void dealCmd();
	static void setNodeConfig(int nodenum,int startNode);
//private:
	const static int REAL;
	const static int VIRTUAL;
	const static int LOCK;
	const static int GPS;
	const static int COAP;
	const static int NB;
	const static int LED;
	
	// add by qiufj at 2019-07-17 
	const static int Temperature;
	const static int FAN;
	const static int Light;
	// end add

	static vector<int> stateList;
	static int print_state_interval; //ms;
	static vector<int> tempList;
	static int startNode;
	static int nodenum;
};
