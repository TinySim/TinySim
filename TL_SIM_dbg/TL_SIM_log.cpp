#include "TL_SIM_log.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


bool TL_SIM_log::isDelayLogEnabled = false;
vector<string> TL_SIM_log::UE_CHANNEL_list;


bool TL_SIM_log::isInUEChannelList(string i_channel)
{
	if(find(UE_CHANNEL_list.begin(), UE_CHANNEL_list.end(), i_channel) != UE_CHANNEL_list.end() )
	{
		return true;
	}
	return false;
}


void TL_SIM_log::set_channel(string i_channel)
{
	this->channel = i_channel;
}

void TL_SIM_log::set_time(int i_time)
{
	this->time = i_time;
}

void TL_SIM_log::set_logMsg(string i_logMsg)
{
	this->logMsg = i_logMsg;
}

void TL_SIM_log::set_state(int i_state)
{
	this->state = i_state ;
}


int TL_SIM_log::get_state()
{
	return this->state;
}

string TL_SIM_log::get_channel()
{
	return this->channel;
}
int TL_SIM_log::get_time()
{
	return this->time;
}
string TL_SIM_log::get_logMsg()
{
	return this->logMsg;
}




