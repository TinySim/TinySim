#ifndef TL_SIM_POWER_UTIL_H_INCLUDED
#define TL_SIM_POWER_UTIL_H_INCLUDED 

// #define S_STATE_NUM 4
// #define S_IDEL 0 
// #define S_SEND 1
// #define S_RECEIVE 2
// #define S_SLEEP 3

#include <string>

using namespace std;

class TL_SIM_power_util
{
public:

	
	static const int total_state_num;


	// about NB state
	static const int S_STATE_NUM;
	static const int S_IDLE;
	static const int S_SEND;
	static const int S_RECEIVE;
	static const int S_SLEEP; 
	static const int S_LISTEN;
	static const int S_SHOTDOWN;

	static string state_name[];
	//static double state_power_consume_NB_BC95[]; // mA
	static double NB_BC95_V;



	// about LOCK state
	static const int LOCK_STATE_NUM;
	static const int LOCK_LOCKing;
	static const int LOCK_UNLOCKing;
	static const int LOCK_Idle;


	static double LOCK_V;

	// about GPS state
	static const int GPS_STATE_NUM;
    static const int GPS_Acquisition;
    static const int GPS_Tracking;
	static const int GPS_Sleep;

	static double GPS_V;



	// about COAP state
	static const int COAP_STATE_NUM;
	static const int COAP_Send;
	static const int COAP_Sleep;

	static double COAP_V;

	// about LED state
	/*
	*  refreshed by qiufujian at 2018-10-13 15:03
	*/
	static const int LED_STATE_NUM;
	static const int LED_ON;
	static const int LED_OFF;
	
	static double LED_V;


	// about Temperature state
	/*
	*  refreshed by qiufujian at 2019-07-16 17:34
	*/
	static const int Temperature_STATE_NUM;
	static const int Temperature_Work;
	static const int Temperature_Idle;
	
	static double Temperature_V;

	// about FAN state
	/*
	*  refreshed by qiufujian at 2019-07-16 17:34
	*/
	static const int FAN_STATE_NUM;
	static const int FAN_ON;
	static const int FAN_OFF;
	
	static double FAN_V;

	// about Light state
	/*
	*  refreshed by qiufujian at 2019-07-16 17:34
	*/
	static const int Light_STATE_NUM;
	static const int Light_Work;
	static const int Light_Idle;
	
	static double Light_V;


	static double state_power_consume[];
	static string POWER_CHANNEL;
	static string power_regex;

};








#endif