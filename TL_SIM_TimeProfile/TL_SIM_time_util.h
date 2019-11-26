#ifndef TL_SIM_TIME_UTIL_H_INCLUDED
#define TL_SIM_TIME_UTIL_H_INCLUDED 


#include <string>

using namespace std;

class TL_SIM_time_util
{
public:
	static const int S_STATE_NUM;
	static const int S_PKT_SEND;
	static const int S_PKT_RECEIVED;
	static const int S_PKT_PEND;

	static const int Mobile_Send_Request;
	static const int LOCK_OPEN;


	static string state_name[];
	static string MY_TIME_CHANNEL;
	static string TIME_CHANNEL;

	static string my_time_regex;
	static string time_regex;
	static string time_regex_muti;


};




#endif