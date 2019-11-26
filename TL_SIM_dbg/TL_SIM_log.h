#ifndef TL_SIM_LOG_H_INCLUDED
#define TL_SIM_LOG_H_INCLUDED 

#include <string>
#include <vector>


using namespace std;

class TL_SIM_log
{
public:
	TL_SIM_log(){}
	~TL_SIM_log(){}






	void set_channel(string);
	void set_time(int);
	void set_logMsg(string);
	void set_state(int);


	string get_channel();
	int get_time();
	string get_logMsg();
	int get_state();

	static bool isDelayLogEnabled;// used for switching between simulated event and real event.
	static vector<string> UE_CHANNEL_list; // used for filtering the log written by  devepors. 

	static bool isInUEChannelList(string i_channel);

private:
	string channel;
	int time;
	int state; 
	string logMsg;

};



#endif