#ifndef TL_SIM_TIMEP_H_INCLUDED
#define TL_SIM_TIMEP_H_INCLUDED 



#include <string>
#include "TL_SIM_time_node.h"
#include "UE.h"
#include <vector>

using namespace std;


class TL_SIM_timep
{
public:
	TL_SIM_timep(){}
	~TL_SIM_timep(){}

	static vector<TL_SIM_time_node> node_time_list;


	static void add_node_time_watch(TL_NB_UE& i_ue);
	static void do_my_profile_time(string buf);
	static void do_profile_time(string buf);
	static bool get_time_node(int moteid, TL_SIM_time_node& node_time);
	// static int get_time_node_index(int moteid);
	static void update_node_time(TL_SIM_time_node& node_time);



	static void print_ave_pkt_delay_nodelist(const vector<int>& nodeList);
	static void print_ave_pkt_delay_consume_all();
	static void get_all_moteid(vector<int>& nodeList);
	static string getCurrentTime();

};



#endif