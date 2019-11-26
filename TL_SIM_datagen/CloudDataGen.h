#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <future>
#include <iostream>
#include <sstream>

#include "TL_COAP.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_schedule.h"


//#include 

class CloudDataGen{
private:
	int event_num;  
	TL_COAP coap;
	int nodenum;
	int startNode;
	long int rand_seed;

	int Mobile_Id = 20001;// simulated mobile device ID. Now is static and fixed.
public:
	CloudDataGen();
	CloudDataGen(int nodenum,int startNode);
	~CloudDataGen(){}
	void setEventNum(int num); // quantity of open lock requests per minute.
	int getRandBySelfSeed();
	int getRandByTime();
	int getRandLockTIme();
	void executeRequestUnlock(string event);
	void executeEvent(string event);
	void loop(int node_id, int lastInterval, int lastTime);

	int getMobileId();
};
