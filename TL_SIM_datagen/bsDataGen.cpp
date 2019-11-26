#include "bsDataGen.h"
#include "TL_SIM_schedule.h"
#include "TL_radio_trace.h"
#include "TL_NB.h"
#include "UE.h"
#include "TL_SIM_timep.h"
#include "TL_SIM_powerp.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>

using namespace std;



// TL_NB::Message bsDataGen::outMessage("open lock!", TL_radio_trace::bs.get_moteid());


void bsDataGen::BSEventInitial()
{

	// if(TL_radio_trace::ueList.size() == 0)
	// {
	// 	cout << "[ERROR] The topo file must be readed before generate the BS event!, exit." << endl;
	// 	exit(1); 
	// }
	// BSEventGen();
	// TL_radio_trace::bs.set_bootTime(0);
	// TL_SIM_timep::add_node_time_watch(TL_radio_trace::bs);
	// TL_SIM_powerp::add_node_power_watch(TL_radio_trace::bs);
}


void bsDataGen::BSEventGen()
{
	// int a =10, b=100, c=TL_radio_trace::ueList.size()-1, d = 0;
	// int EventTime = (rand() % (b-a))+ a; 	
	// // int EventTime = 1110;
	// int NodeIndex = (rand() % (c-d+1))+ d; 

	// cout << "[bsDataGen], execute at" << sim_scheduler::currentTime + EventTime << ", to node" << TL_radio_trace::ueList[NodeIndex].get_moteid() << endl;

	// // TL_radio_trace::ueList[NodeIndex].set_state(TL_SIM_power_util::S_LISTEN);

	// TL_NB::Message outMessage("open lock!", TL_radio_trace::bs.get_moteid(), TL_radio_trace::ueList[NodeIndex].get_moteid());

	// // TL_NB_UE bs = TL_radio_trace::bs;
	// // TL_NB_UE ue = TL_radio_trace::ueList[NodeIndex];

	// //@ event instrumentation, need file static analysis.
	// std::shared_future<void> s1 = async(launch::deferred, &TL_NB_UE::sendMessage, &TL_radio_trace::bs ,outMessage, TL_radio_trace::ueList[NodeIndex]);
	// // std::shared_future<void> s1 = async(launch::deferred, &TL_NB_UE::sendMessage, &bs ,outMessage, ue);
	// sim_event e1( sim_scheduler::currentTime + EventTime, TL_radio_trace::bs.get_moteid(), s1);
	// sim_scheduler::sim_queue_insert(e1);
	// cout << "[bsDataGen] run end." << endl;
	// std::shared_future<void> s1 = async(launch::deferred, &node::loop, this);
	// sim_event e1(sim_scheduler::currentTime + ue.get_listenTime(), ue.get_moteid(), s1); 
	// // e1.set_priority(sim_scheduler::LOW_PRO);// pending the evnet that can be changed.
	// sim_scheduler::sim_queue_insert(e1);
	// std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::sendMessage, this,  (msg) , (dst));
	// sim_event e(nextNodeWakupTime + 1, this->get_moteid(), s); // insert the event time just 1 ms larger than the listen time.
	// sim_scheduler::sim_queue_insert(e);
}


