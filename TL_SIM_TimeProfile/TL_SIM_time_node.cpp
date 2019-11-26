#include "TL_SIM_time_node.h"
#include <map>
#include <vector>

using namespace std;






void TL_SIM_time_node::set_moteid(int i_moteid)
{
	this->moteid = i_moteid; 
}


int TL_SIM_time_node::get_moteid()
{
	return this->moteid;
}
void TL_SIM_time_node::set_random_access_delay(int random_access_delay)
{
	this->random_access_delay = random_access_delay;
}
int TL_SIM_time_node::get_random_access_delay()
{
	return this->random_access_delay;
}



void TL_SIM_time_node::set_pkt_delay(int key, int i_pkt_delay)
{
	this->total_pkt_delay[key] = i_pkt_delay;
	// this->total_pkt_delay = i_pkt_delay;
}
int TL_SIM_time_node::get_pkt_delay(int key)
{
	return this->total_pkt_delay[key];
}

void TL_SIM_time_node::set_intend_pkt_num(int key,int intend_pkt_num)
{
	this->intend_pkt_num[key] = intend_pkt_num;
}
int TL_SIM_time_node::get_intend_pkt_num(int key)
{
	return this->intend_pkt_num[key];
}
void TL_SIM_time_node::set_tx_pkt_num(int key,int tx_pkt_num)
{
	this->tx_pkt_num[key] = tx_pkt_num;
}
int TL_SIM_time_node::get_tx_pkt_num(int key)
{
	return this->tx_pkt_num[key];
}

// void TL_SIM_time_node::set_pre_state(int key,int state)
// {
// 	this->pre_state[key] = state;
// }
// int TL_SIM_time_node::get_pre_state(int key)
// {
// 	return this->pre_state[key];
// }
// void TL_SIM_time_node::set_pre_time(int key,int time)
// {
// 	this->pre_time[key] = time; 
// }
// int TL_SIM_time_node::get_pre_time(int key)
// {
// 	return this->pre_time[key];
// }

void TL_SIM_time_node::set_pre_state(int state)
{
	this->pre_state = state;
}
int TL_SIM_time_node::get_pre_state()
{
	return this->pre_state;
}
void TL_SIM_time_node::set_pre_time(int time)
{
	this->pre_time = time; 
}
int TL_SIM_time_node::get_pre_time()	
{
	return this->pre_time;
}


void TL_SIM_time_node::set_open_lock_pre_state(int state)
{
	this->open_lock_pre_state = state;
}
int TL_SIM_time_node::get_open_lock_pre_state()
{
	return this->open_lock_pre_state;
}

void TL_SIM_time_node::set_open_lock_pre_time(int time)
{
	this->open_lock_pre_time = time; 
}
int TL_SIM_time_node::get_open_lock_pre_time()	
{
	return this->open_lock_pre_time;
}



// void TL_SIM_time_node::add_pkt_delay_item(int key)
// {
// 	this->
// }
// void TL_SIM_time_node::add_intend_pkt_num_item(int key)
// {

// }
// void TL_SIM_time_node::add_tx_pkt_num_item(int key)
// {

// }


// randomly choose a  variable to check whether there is the key 
bool TL_SIM_time_node::isKeyInMap(int key)
{
  	map<int, int>::iterator it = total_pkt_delay.find(key);
	if(it != total_pkt_delay.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}


void TL_SIM_time_node::get_all_dst_moteid(vector<int>& nodeList)
{
  	for(map<int, int>::iterator it = total_pkt_delay.begin(); it != total_pkt_delay.end(); it++)
  	{
  		nodeList.push_back(it->first);
  	}

}
