#include "TL_SIM_power_util.h"
#include <string>

using namespace std;


// NB state
const int TL_SIM_power_util::S_STATE_NUM = 6;

const int TL_SIM_power_util::S_IDLE = 0;
const int TL_SIM_power_util::S_SEND = 1;
const int TL_SIM_power_util::S_RECEIVE = 2;
const int TL_SIM_power_util::S_SLEEP = 3;
const int TL_SIM_power_util::S_LISTEN = 4;
const int TL_SIM_power_util::S_SHOTDOWN = 5;

// LOCK state
const int TL_SIM_power_util::LOCK_STATE_NUM = 3;

const int TL_SIM_power_util::LOCK_LOCKing = 6;
const int TL_SIM_power_util::LOCK_UNLOCKing = 7;
const int TL_SIM_power_util::LOCK_Idle = 8;

// GPS state
const int TL_SIM_power_util::GPS_STATE_NUM = 3;

const int TL_SIM_power_util::GPS_Acquisition = 9;
const int TL_SIM_power_util::GPS_Tracking = 10;
const int TL_SIM_power_util::GPS_Sleep = 11;

// COAP state
const int TL_SIM_power_util::COAP_STATE_NUM = 2;

const int TL_SIM_power_util::COAP_Send = 12;
const int TL_SIM_power_util::COAP_Sleep = 13;

// LED state
const int TL_SIM_power_util::LED_STATE_NUM = 2;

const int TL_SIM_power_util::LED_ON = 14;
const int TL_SIM_power_util::LED_OFF = 15;


// Temperature state
const int TL_SIM_power_util::Temperature_STATE_NUM = 2;

const int TL_SIM_power_util::Temperature_Work = 16;
const int TL_SIM_power_util::Temperature_Idle = 17;

// FAN state
const int TL_SIM_power_util::FAN_STATE_NUM = 2;

const int TL_SIM_power_util::FAN_ON = 18;
const int TL_SIM_power_util::FAN_OFF = 19;


// Light state
const int TL_SIM_power_util::Light_STATE_NUM = 2;

const int TL_SIM_power_util::Light_Work = 20;
const int TL_SIM_power_util::Light_Idle = 21;



const int TL_SIM_power_util::total_state_num = 22;  // not display coap state



//string TL_SIM_power_util::state_name[] = {"NB_idle", "send", "receive", "sleep", "listen", "shutdown"};
string TL_SIM_power_util::state_name[] = {
				"NB_idle", "NB_send", "NB_receive", "NB_sleep", "NB_listen", "NB_shutdown", // NB state
				"LOCK_CLOSE","LOCK_OPEN","LOCK_Idle",										// LOCK state
				"GPS_Acquisition","GPS_Tracking","GPS_Sleep", 								// GPS state
				"COAP_Send", "COAP_Sleep", 													// COAP state
				"LED_ON","LED_OFF",															// LED state
				"Temperature_Work","Temperature_Idle",										// Temperature state
				"FAN_ON","FAN_OFF",															// FAN state
				"Light_Work","Light_Idle",														// Light state
			};


double TL_SIM_power_util::state_power_consume[] = {
	            1.04, 3.173, 3.173, 0.004727, 3.173, 0,       // NB_BC95 module mA
	            20.25, 20.56, 0,                              // LOCK module mA
	            40, 35, 0.005,								  // GPS module mA
	            0.2 , 0,                                      // COAP module mA
	            20 , 0 ,									  // LED module mA
	            1.12 , 0.34,								  // Temperature module mA
	            300 , 0,								  	  // FAN module mA	
	            0.89 , 0.35,								  // Light module mA						
	        };

//double TL_SIM_power_util::state_power_consume_NB_BC95[] = {1.04, 3.173, 3.173, 0.004727, 3.173, 0}; //mA

double TL_SIM_power_util::NB_BC95_V = 3.8;    //V
double TL_SIM_power_util::LOCK_V    = 3.8;    //V
double TL_SIM_power_util::GPS_V     = 3; 	  //V
double TL_SIM_power_util::COAP_V    = 3.3; 	  //V
double TL_SIM_power_util::LED_V    = 3; 	  //V
double TL_SIM_power_util::Temperature_V    = 3.8; 	  //V
double TL_SIM_power_util::FAN_V    = 3.8; 	  //V
double TL_SIM_power_util::Light_V  = 3.8; 	  //V


string TL_SIM_power_util::POWER_CHANNEL = "POWER_CHANNEL";
string TL_SIM_power_util::power_regex = R"(TIME:(.+), NODE:(.+), STATE:(.+)\.)";

