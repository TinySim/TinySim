#ifndef TL_SIM_SCHEDULE_H_INCLUDED
#define TL_SIM_SCHEDULE_H_INCLUDED

#include <future>
#include <string>
#include <vector>
#include <iostream>
#include "TL_NB.h"
#include "UE.h"

#include "sim_event_util.h"

class TL_NB_UE;


using namespace std;

class sim_event{

public:

	sim_event(){}
	sim_event(int time,int mote, std::shared_future<void> handler)
		:  m_time(time), m_mote(mote), m_handler(handler){}

	~sim_event(){}



	void set_time(int time)
	{
		this->m_time = time;
	}

	int get_time()
	{
		return m_time;
	}

	int get_mote()
	{
		return m_mote;
	}

	void set_mote(int mote)
	{
		this->m_mote = mote;
	}
	
	void set_handler(std::shared_future<void> handler)
	{
		this->m_handler = handler;
	}

	std::shared_future<void> get_handler()
	{
		// cout << "[ToExecu]" << get_mote() << ",TIME:" <<  get_time() << endl;
		return m_handler;
	}

	void set_priority(int priority)
	{
		this->priority = priority; 
	}

	int get_priority()
	{
		return this->priority;
	}

	void set_handler_name(string names)
	{
		this->handler_name = names;
	}

	string get_handler_name()
	{
		return this->handler_name;
	}


private:
	int m_time;	
	int m_mote;
	std::shared_future<void> m_handler;
	int priority = sim_event_util::HIGH_PRIORITY;

	string handler_name; // record the function name that will be executed by this handler.

};



class sim_dispatch_event
{
public:

	sim_dispatch_event(){}
	sim_dispatch_event(int m_st_time,int m_pkt_on_air_time, int m_tx_node_id, int m_rx_node_id , \
		TL_NB::Message m_msgretx, TL_NB::Message m_msgrece, TL_NB_UE* m_dst,  TL_NB_UE* m_src,\
		std::shared_future<void> m_retx_hanlder, std::shared_future<void> m_rece_hanlder):  
		st_time(m_st_time), pkt_on_air_time(m_pkt_on_air_time), tx_node_id(m_tx_node_id), rx_node_id(m_rx_node_id), \
		 msgretx(m_msgretx),  dst(m_dst),src(m_src), msgrece(m_msgrece), retx_hanlder(m_retx_hanlder), rece_hanlder(m_rece_hanlder)\
		  {

		  	// this->predetermined_handler = -1;
		  }

	~sim_dispatch_event(){}


	int get_st_time()
	{
		return this->st_time;
	}
	int get_pkt_on_air_time()
	{
		return this->pkt_on_air_time;
	}

	int get_rx_node_id()
	{
		return this->rx_node_id;
	}

	int get_tx_node_id()
	{
		return this->tx_node_id;
	}
	TL_NB::Message get_msgretx()
	{
		return this->msgretx;
	}
	TL_NB::Message get_msgrece()
	{
		return this->msgrece;
	}
	TL_NB_UE* get_dst()
	{
		return this->dst;
	}
	TL_NB_UE* get_src()
	{
		return this->src;
	}


	int get_predetermined_handler()
	{
		return this->predetermined_handler;
	}
	void set_predetermined_handler(int predetermined_handler_m)
	{
		this->predetermined_handler = predetermined_handler_m;
	}

	inline bool operator == (const sim_dispatch_event &sde) const
	{
		if(this->tx_node_id == sde.tx_node_id && this->rx_node_id == sde.rx_node_id 
			&& this->pkt_on_air_time == sde.pkt_on_air_time && this->st_time == sde.st_time )
			return true;
		return false;
	}

	std::shared_future<void> get_retx_handler()
	{
		return this->retx_hanlder;
	}

	std::shared_future<void> get_recv_handler()
	{
		return this->retx_hanlder;
	}


private:

	int st_time;
	int pkt_on_air_time;
	int rx_node_id; // we should find potential collided neighbors based rx. 
	int tx_node_id; // we should perform collision decision based tx.
	TL_NB::Message msgretx; 
	TL_NB_UE* dst;   
	TL_NB_UE* src;   

	TL_NB::Message msgrece;

	std::shared_future<void> retx_hanlder;
	std::shared_future<void> rece_hanlder;

	int predetermined_handler=-1; // for recording that this event has been dealed.  1: means it's a collision.

};


class sim_scheduler
{

public:
	static std::vector<sim_event> q;
	static int currentTime;
	static vector<sim_event> pendedQ; // pended queue for low priority event execute.

	static vector<sim_dispatch_event> dispatchQ;

	static int executedEventsCount;
	static int retxEventsCout;
	static long int max_execute_time;
	static int timeConsumeDispatchQ;



	static void sim_queue_init();


	static void sim_queue_insert(sim_event& event);

	static void sim_queue_insert_delay(sim_event& event);

	static int sim_queue_size();

	static sim_event sim_queue_pop();

	static bool sim_queue_is_empty();

	// @ true: find event with moteid, and put it into oute.
	// static bool sim_queue_pop_pend_event(int moteid, sim_event& oute)
	// {
	// 	for(int i=0 ;i< pendedQ.size(); i++)
	// 	{
	// 		if( pendedQ[i].get_mote() == moteid)
	// 		{
	// 			oute = pendedQ[i];
	// 			// pendedQ.erase(pendedQ[i]); 
	// 			return true;
	// 		}			
	// 	}
	// 	return false;
	// }

	// @ upgrade the pend event and move into the queue, when the formar event has been done. 
	// static void sim_queue_upgrade_pend_event_to_queue(sim_event& dstE)
	// {	
	// 	for(std::vector<sim_event>::iterator iter = q.begin()-1; iter != q.end(); iter++ )
	// 	{
	// 		if( iter->get_mote() == moteid)
	// 		{
	// 			dstE.set_handler(iter->get_handler());
	// 			// sim_event pend_event(iter->get_time(), iter->get_mote(), iter->get_handler());
	// 			// pendedQ.push_back(pend_event);
	// 			q.erase(iter); 
	// 			break;
	// 		}
	// 	}
	// 	dstE.set_priority(sim_event_util::HIGH_PRIORITY);
	// 	sim_queue_insert(dstE);
	// }

	// @ upgrade the pend event and move into the queue, when the formar event has been done. 
	static void sim_queue_upgrade_pend_event_to_queue(int moteid, int runTime);


	// @ [ISSUE] currently, there is only one pended event support.
	static void sim_queue_pend_event(int moteid);


	static void sim_print_queue();
	static void sim_print_queue_maxCount(int maxC);

	static void sim_print_pendQ();

	static void sim_print_dispatchQ();


	static int find_nodeid_from_intqueue(int nodeid, vector<int> QQ );
	static int find_dispatchevent_index_from_dispatchQ_by_node_id(int nodeid);

	static void rx_dispatcher(int st_time, int pkt_on_air_time, int tx_node_id,  int rx_node_id, 
		std::shared_future<void> handler_retxMessage, TL_NB::Message msgretx, TL_NB_UE* dst,   TL_NB_UE* src,
		std::shared_future<void> handler_receiveMessage, TL_NB::Message msgrece);

	static long int get_local_time();

};




#endif


