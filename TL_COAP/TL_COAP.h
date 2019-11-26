#ifndef TL_COAP_h
#define TL_COAP_h

#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <future> 
#include <chrono> 
#include <cstdio>
#include <sys/time.h>

#include <time.h>
#include <unistd.h>

#include "TL_SIM_dbg.h"

extern "C"{
   #include "client.h"
}
using namespace std;

class TL_COAP{
private:
	
	int  delay;   // ms
	int  interval;
	char ip[30];
	char port[6];
	char result_info[1024]; // 1kB
	string macType;
	int ueId;  // 网联网节点的ID add by qiufj at 2019-07-17
	int bsId;  // 基站ID  

public:
	TL_COAP();
	char* getResult();
	int   getDelay();
	int   getInterval();
	char* getIP();
	void setMacType(string mactype);
	string getMacType();
	void connect(string ip,string port);
	void send(int node_id, double lat,double lon);
	void send(int node_id,string str);
	void sendData(string url);
	void sendIdByPut(int node_id,int close);
	void roundSend(int node_id);
	string getTimeStamp(); // used for test netowrk delay
	void setUEId(int ueId);
	int getUEId();
	void setBSId(int id);
	void setBSIdByUEid(int ueId);

	int getBSId();
};
#endif

int getRandomDelay();
