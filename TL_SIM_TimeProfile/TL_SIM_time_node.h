#ifndef TL_SIM_TIME_NODE_H_INCLUDED
#define TL_SIM_TIME_NODE_H_INCLUDED 

#include "TL_SIM_time_node.h"
#include <map>
#include <vector>
#include <iostream>

using namespace std;


class TL_SIM_time_node
{
public:
	TL_SIM_time_node(){}
	~TL_SIM_time_node(){}

	void set_moteid(int);	
	int get_moteid();

	void set_random_access_delay(int);
	int get_random_access_delay();

	// void set_pkt_delay(int);
	// int get_pkt_delay();
	// void set_intend_pkt_num(int);
	// int get_intend_pkt_num();
	// void set_tx_pkt_num(int);
	// int get_tx_pkt_num();

	void set_pre_state(int);
	void set_pre_time(int);

	int get_pre_state();
	int get_pre_time();

	void set_open_lock_pre_state(int);
	void set_open_lock_pre_time(int);
	
	int get_open_lock_pre_state();
	int get_open_lock_pre_time();



	void set_pkt_delay(int, int);
	int get_pkt_delay(int);

	void set_intend_pkt_num(int, int);
	int get_intend_pkt_num(int);

	void set_tx_pkt_num(int, int);
	int get_tx_pkt_num(int);

	// void set_pre_state(int, int);
	// void set_pre_time(int, int);
	// int get_pre_state(int);
	// int get_pre_time(int);

	bool isKeyInMap(int);
	void get_all_dst_moteid(vector<int>& nodeList);

	// void add_pkt_delay_item(int);
	// void add_intend_pkt_num_item(int);
	// void add_tx_pkt_num_item(int);




private:
	int moteid;

	int random_access_delay = 0; // pended, due to have no way to verify.


	map<int, int> total_pkt_delay;
	map<int, int> intend_pkt_num;
	map<int, int> tx_pkt_num;

	// vector<double> state_power_consume;
	// vector<int> state_time_consume;
	// double state_power_consume[TL_SIM_power_util::S_STATE_NUM] = {0};
	// int state_time_consume[TL_SIM_power_util::S_STATE_NUM] = {0};
	int pre_time;
	int pre_state;

	int open_lock_pre_time;
	int open_lock_pre_state;

	// map<int, int> pre_state;
	// map<int, int> pre_time;
	
};







#endif












