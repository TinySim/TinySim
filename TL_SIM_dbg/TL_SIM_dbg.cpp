#include "TL_SIM_dbg.h"
#include "TL_SIM_log.h"
#include "TL_SIM_powerp.h"
#include "TL_SIM_timep.h"
#include "TL_SIM_time_util.h"
#include <string>
#include <stdarg.h>
#include <regex>
#include <vector>
#include <iostream>
#include <fstream>



using namespace std;


void TL_SIM_dbg::dbg_filter(const string& path, const string& regStr, vector<string>& outp) 
{
    // std::regex re(R"((.+?)(\.[^.]+)?)");
    // std::regex re(R"(TIME:(.+), NODE:(.+), STATE:(.+)\.)");
    std::regex re(regStr);
    std::smatch results;

    std::regex_search(path, results, re);

    // vector<string> outp;
    for(int i=0 ;i< results.size(); i++)
    {
    	string inst(results[i]);
    	outp.push_back(inst);
    }
    // return outp;

}


// vector<string> TL_SIM_dbg::dbg_filter(const string& path, const string& regStr) 
// {
//     // std::regex re(R"((.+?)(\.[^.]+)?)");
//     // std::regex re(R"(TIME:(.+), NODE:(.+), STATE:(.+)\.)");
//     std::regex re(regStr);
//     std::smatch results;

//     std::regex_search(path, results, re);

//     vector<string> outp;
//     for(int i=0 ;i< results.size(); i++)
//     {
//     	outp.push_back(results[i]);
//     }
//     return outp;

// }

void TL_SIM_dbg::dbg(string channel, const char* format, ...)
{

	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer,format, args);
	string buf(buffer);
	va_end(args);


	if (TL_SIM_log::isInUEChannelList(channel))
	{
		// ofstream ofs("../result/application.log",ios::out);
	    // cout << getTinySimTime()<<" "<< buf;
       	// ofs.close();

       	// ofstream ofs2("../result/" + getTinySimTime() + ".log",ios::out);
	 	cout << "[" << channel << "]"  << getTinySimTime()<<" "<< buf << endl;
	 	// cout << buf << endl;
       	// ofs2.close();
	}
	if(channel == TL_SIM_power_util::POWER_CHANNEL)
	{
		TL_SIM_powerp::do_profile_power(buf);
	}
	else if(channel == TL_SIM_time_util::TIME_CHANNEL)
	{
		TL_SIM_timep::do_profile_time(buf);
	}
	else if(channel == TL_SIM_time_util::MY_TIME_CHANNEL){
		TL_SIM_timep::do_my_profile_time(buf);
	}

    if(TL_SIM_log::isDelayLogEnabled == false)
    {

    }
    else
    {

    }
}



void TL_SIM_dbg::dbg_add_channel(string channel)
{
	TL_SIM_log::UE_CHANNEL_list.push_back(channel);
}


void TL_SIM_dbg::dbg_remove_channel(string channel)
{
	for(vector<string>::iterator it = TL_SIM_log::UE_CHANNEL_list.begin(); it != TL_SIM_log::UE_CHANNEL_list.end(); )
	{
		if( *it == channel )
		{
			it = TL_SIM_log::UE_CHANNEL_list.erase(it);
		}
		else
		{
			it++;
		}
	}
}
string TL_SIM_dbg::getTinySimTime(){
        int tinysim_time = sim_scheduler::currentTime;
        int millisecond = tinysim_time % 1000 ;
        tinysim_time /= 1000;
        int second = tinysim_time % 60;
        tinysim_time /= 60;
        int minute = tinysim_time % 60;
        tinysim_time /= 60;
        int hour = tinysim_time % 60 + 6;

        char time_str[30];
        sprintf(time_str,"%02d%s%02d%s%02d%s%03d",hour,":",minute,":",second,".",millisecond);
        return (string)time_str;
}













