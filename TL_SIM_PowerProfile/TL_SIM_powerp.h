#ifndef TL_SIM_POWERP_H_INCLUDED
#define TL_SIM_POWERP_H_INCLUDED 


#include <string>
#include "TL_SIM_power_node.h"
#include "TL_SIM_schedule.h"
#include "UE.h"
#include <map>

using namespace std;


class TL_SIM_powerp
{
public:
	TL_SIM_powerp(){}
	~TL_SIM_powerp(){}


	static vector<TL_SIM_power_node> node_p_list;


	static void add_node_power_watch(TL_NB_UE& i_ue);
	static void do_profile_power(string buf);
	// static TL_SIM_power_node& get_power_node(int moteid);
	static bool get_power_node(int moteid, TL_SIM_power_node& node_power);
	// static TL_SIM_power_node& get_power_node(int moteid, bool& isFind);

	// static bool get_power_node(int moteid, TL_SIM_power_node* node_power);
	static int get_power_node_index(int moteid);
	static void update_node_power(TL_SIM_power_node& node_power);

	static void print_state_time_consume_nodelist(const vector<int>& nodeList);
	static void print_state_time_consume_all();
	static void print_state_power_consume_nodelist(const vector<int>& nodeList);
	static void print_state_power_consume_all();
	
	static void get_all_moteid(vector<int>& nodeList);

	static void print_node_state();  //called in TL_SIM_mode.cpp 

	static string getCurrentTime();

	// enrich in 2018-10-17 by qiufujian.
	static void setVarToMark();
	//static string getTinySimTime();

private:
	static map<int,string> state_map;
	static int mark[22];  // 22 is the total num of hardware state,refer to TL_SIM_power_util.h

};



#endif







