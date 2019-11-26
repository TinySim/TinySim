#include <algorithm>
#include "TL_SIM_schedule.h"
#include "TL_radio_csma.h"
#include "TL_radio.h"
#include <sstream>
#include <math.h>
#include <sys/time.h>




std::vector<sim_event> sim_scheduler::q;
int sim_scheduler::currentTime = 0; // us as the scale.
vector<sim_event> sim_scheduler::pendedQ; 

// list of pended rx events. dealing with rx dispatch. For simulating collisions.
std::vector<sim_dispatch_event> sim_scheduler::dispatchQ;

long int sim_scheduler::max_execute_time;

 int sim_scheduler::executedEventsCount;
int sim_scheduler::retxEventsCout = 0;
int sim_scheduler::timeConsumeDispatchQ = 0;

void sim_scheduler::sim_queue_init()
{}


void sim_scheduler::sim_queue_insert(sim_event& event)
{
	if (q.empty())
	{
		q.push_back(event);
		// currentTime = event.get_time();
		return;
	}

	// cout << "insert" << endl;

	// cout << event.get_mote() << " " << event.get_time() << endl;

	for(std::vector<sim_event>::iterator iter = q.begin(); iter != q.end(); iter++ )
	{
		if (event.get_time() < iter->get_time())
		{
			q.insert(iter, event);
			// currentTime = event.get_time();
			// cout << "currentTime: " << currentTime << endl;
			return;
		}
	}
	q.push_back(event);
	// currentTime = event.get_time();

}

void sim_scheduler::sim_queue_insert_delay(sim_event& event)
{
	event.get_handler().wait();
	currentTime = event.get_time();
}

int sim_scheduler::sim_queue_size()
{
	return q.size();
}

sim_event sim_scheduler::sim_queue_pop()
{


	// sim_scheduler::sim_print_queue_maxCount(5);

	std::vector<sim_event>::iterator iter = q.begin();
	sim_event top_event(iter->get_time(), iter->get_mote(), iter->get_handler());

	if(currentTime > max_execute_time){ // when we pop new event to perform, we shall first check the time.
		top_event.set_mote(-1); // by setting a invalied moteId, as we should exits.
		return top_event;
	}

	currentTime = iter->get_time();

	q.erase(iter); 
	// cout << " [Event C] " << sim_queue_size() << ", after ExeMote:" << q[0].get_mote() <<  endl;

	executedEventsCount++; // increased for tracing the number of executed events.


	return top_event;
}

bool sim_scheduler::sim_queue_is_empty()
{
	return q.empty();
}

// @ true: find event with moteid, and put it into oute.
// bool sim_scheduler::sim_queue_pop_pend_event(int moteid, sim_event& oute)
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
// void sim_scheduler::sim_queue_upgrade_pend_event_to_queue(sim_event& dstE)
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
void sim_scheduler::sim_queue_upgrade_pend_event_to_queue(int moteid, int runTime)
{	
	for(std::vector<sim_event>::iterator iter = pendedQ.begin()-1; iter != pendedQ.end(); iter++ )
	{
		if( iter->get_mote() == moteid)
		{
			// cout << "[UPgrade, before] TIME:" <<  iter->get_time() << ", node:" << iter->get_mote() << endl;
			sim_event upgrade_event(runTime, moteid, iter->get_handler());
			// cout << "[UPgrade, after] TIME:" <<  upgrade_event.get_time() << ", node:" << upgrade_event.get_mote() << endl;
			sim_queue_insert(upgrade_event);
			pendedQ.erase(iter); 
			return;
		}
	}
}


int sim_scheduler::find_dispatchevent_index_from_dispatchQ_by_node_id(int nodeid)
{
	for (int i = 0; i < dispatchQ.size(); ++i)
	{
		if(dispatchQ[i].get_tx_node_id() == nodeid){
			return i;
		}
	}
	return -1;
}

int sim_scheduler::find_nodeid_from_intqueue(int nodeid, vector<int> QQ )
{
	for (int i = 0; i < QQ.size(); ++i)
	{
		if(QQ[i] == nodeid){
			return i;
		}
	}
	return -1;
}


// [cgl] focus on the handler of retxMessage/receiveMessage. Therefore, the parameters are fixed.
void sim_scheduler::rx_dispatcher(int st_time, int pkt_on_air_time, int tx_node_id,  int rx_node_id, 
	std::shared_future<void> handler_retxMessage, TL_NB::Message msgretx, TL_NB_UE* dst,   TL_NB_UE* src,
	std::shared_future<void> handler_receiveMessage, TL_NB::Message msgrece)
{

	int dispatch_startTime = get_local_time(); // to record the time consumed for dispatch process.

	vector<int> nodeIdFlag;

	// 0.append this event into the dispatchQ.
	sim_dispatch_event intemp(st_time, pkt_on_air_time, tx_node_id, rx_node_id, \
	msgretx, msgrece, dst, src,	handler_retxMessage, handler_receiveMessage);



	dispatchQ.push_back(intemp);

	vector<sim_dispatch_event> copy_of_dispatchQ(dispatchQ);

	TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] into dispatch, Qlen: %d. InvokedNoid:%d", dispatchQ.size(), tx_node_id);
	TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] rx_node_id:%d, dstId:%d, srcId:%d", rx_node_id, dst->get_moteid(), src->get_moteid());


	// 1. two checks. for all of the related nodes. 
	// 1.1 is related nodes are in dispatch Q?
	// 1.2 is the start time of related nodes in q is larger than st_time?
	// True: check collisions and determine next states.
	// False:  and deal with other event in dispatchQ. Note that the node should not be included in the above events. 

	for (int i = 0; i < copy_of_dispatchQ.size(); ++i)
	{

		TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] step 1 whether related nodes are ready for collisioncheck");

		sim_dispatch_event one_dispatch = copy_of_dispatchQ[i];

		vector<int> neighborList;
		TL_radio::getNeighborsFromRxId(one_dispatch.get_rx_node_id(), neighborList);

		vector<int> visitFlag(neighborList.size());// flags for recording the nodes passing the check.
		// for 1.1.-- maybe not needed? -- need
		for (int j = 0; j < neighborList.size(); ++j)
		{
			if(find_dispatchevent_index_from_dispatchQ_by_node_id(neighborList[j]) != -1){
				visitFlag[j] = 1;
			}
		}

		//for 1.2
		bool isReadyForCollisionCheck = true;
		int rxPktArriveTime = one_dispatch.get_st_time() + one_dispatch.get_pkt_on_air_time();

		for (int j = 0; j < q.size(); ++j)
		{
			int tempNodeid = q[j].get_mote();
			int tempStTime = q[j].get_time();

			// If it is the neighbor nodes. It should be in dispatchQ or the start time is larger.otherwise, it is not ready.
			int TmpIndex = find_nodeid_from_intqueue(tempNodeid,neighborList);
			if( TmpIndex != -1 &&  visitFlag[TmpIndex] != 1 && rxPktArriveTime > tempStTime )
			{
				isReadyForCollisionCheck = false;
				break;
			}

			if(tempStTime >= rxPktArriveTime){ // The events are sorted with time. The later events no need to check.
				break;
			}


		}
		// [@] not ready for checking collisions, and pick next packets.
		// TODO: optimize? remove all related nodes in dispatchQ?
		if(isReadyForCollisionCheck==false){
			continue;  
		}

		// 2. determine next states.
		// 2.1 weak link
		// 2.2 collisions
		// 
		// True: retx.
			// -- label collisions for related nodes. 
		// 
		// All False: 
			// no collision label: receive; otherwise: retx.

		TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] step 2 have collision ready nodes");


		vector<int> collisionCheckNodeList;
		for (int j = 0; j < visitFlag.size(); ++j)
		{
			if(visitFlag[j] == 1){
				collisionCheckNodeList.push_back(neighborList[j]);
			}
		}
		// TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] step 2.2 collisionCheckNodeList");
		// for (int j = 0; j < collisionCheckNodeList.size(); ++j)
		// {
		// 	cout << collisionCheckNodeList[j] << ",";
		// }	
		// cout << endl;


		// 2.2 check whether this node has collisions with other nodes. 
		int currentTxNodeId = one_dispatch.get_tx_node_id();

		int currentRxTime = one_dispatch.get_pkt_on_air_time() + one_dispatch.get_st_time();
		int currentDispatchIndex = find_dispatchevent_index_from_dispatchQ_by_node_id(currentTxNodeId);

		for(int j=0 ; j < collisionCheckNodeList.size(); j++){
			if (currentTxNodeId == collisionCheckNodeList[j]){
				continue;
			}

			int targetDispatchIndex = find_dispatchevent_index_from_dispatchQ_by_node_id(collisionCheckNodeList[j]);
			sim_dispatch_event targetDispatch = dispatchQ[targetDispatchIndex] ;
			int targetRxTime = targetDispatch.get_pkt_on_air_time() + targetDispatch.get_st_time();

			if(one_dispatch.get_st_time() < targetRxTime &&  targetDispatch.get_st_time() <currentRxTime ){
				dispatchQ[targetDispatchIndex].set_predetermined_handler(1);// label all related nodes as collided
				dispatchQ[currentDispatchIndex].set_predetermined_handler(1);				
			}
		}
		// for 2.1
		TL_radio_link* link = TL_radio::findLink(one_dispatch.get_tx_node_id(), one_dispatch.get_rx_node_id());
		double ber = 0.0;
		if(link != NULL)
			double ber = link->get_erRate();
		else{
			ber = 0.0;
		}
		// cout << "[DBG], into  sendmsg, test1\n";
		// cout <<  msg.get_payload() << "," << msg.get_moteid() << endl;
		int msgLen = dispatchQ[currentDispatchIndex].get_msgretx().get_payload().size() * 8;
		// cout << "[DBG], into  sendmsg, test2\n";
		double pktEr = 1 - pow((1 - ber), msgLen);
		double errorPro = rand() / double(RAND_MAX);
		if (errorPro < pktEr){
			dispatchQ[currentDispatchIndex].set_predetermined_handler(1);// for weak link, set collisions.
		}


		// sim_print_dispatchQ();

		TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] currentDispatchIndex: %d, %p, %p",  currentDispatchIndex,
			dispatchQ[currentDispatchIndex].get_retx_handler(), dispatchQ[currentDispatchIndex].get_retx_handler());


		// 2.3 deal with the next state of the current dispatch event.
		ostringstream ost;
		if(dispatchQ[currentDispatchIndex].get_predetermined_handler() == 1){
			dispatchQ[currentDispatchIndex].get_msgretx().set_txNum(dispatchQ[currentDispatchIndex].get_msgretx().get_txNum() + 1);

			int backoffTime = TL_radio_csma::slotTime + (TL_radio_csma::maxBackOffTime-TL_radio_csma::slotTime)*(rand()*1.0/RAND_MAX);

		// TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] slotTime %d, %d, %d", TL_radio_csma::slotTime, TL_radio_csma::maxBackOffTime-TL_radio_csma::slotTime, backoffTime);


			// int NextTime =  sim_scheduler::currentTime + dispatchQ[currentDispatchIndex].get_st_time() + dispatchQ[currentDispatchIndex].get_pkt_on_air_time() + backoffTime ;

			int NextTime =  dispatchQ[currentDispatchIndex].get_st_time() + dispatchQ[currentDispatchIndex].get_pkt_on_air_time() + backoffTime ;


		TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] step 2.3 perform retx at:%d", NextTime);

			// std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::retxMessage, 
			// 								dispatchQ[currentDispatchIndex].get_src(), 
			// 									dispatchQ[currentDispatchIndex].get_msgretx(),  
			// 									*dispatchQ[currentDispatchIndex].get_dst());

			// sim_event e(NextTime, dispatchQ[currentDispatchIndex].get_tx_node_id(), s);
			sim_event e(NextTime, dispatchQ[currentDispatchIndex].get_tx_node_id(), dispatchQ[currentDispatchIndex].get_retx_handler());
			// sim_event e(sim_scheduler::currentTime + backoffTime,   dispatchQ[currentDispatchIndex].get_tx_node_id(), dispatchQ[currentDispatchIndex].get_retx_handler());


			ost << "TL_NB_UE::retxMessage@[dst:" << dispatchQ[currentDispatchIndex].get_rx_node_id() << "]";
			e.set_handler_name(ost.str());
			sim_scheduler::sim_queue_insert(e);

			TL_SIM_dbg::dbg("TL_SIM_schedule", " [TL_SIM_schedule]  %d retx, to dst:%d", 
				dispatchQ[currentDispatchIndex].get_tx_node_id(), 
				dispatchQ[currentDispatchIndex].get_rx_node_id());
		}
		else{


			// int NextTime = sim_scheduler::currentTime + dispatchQ[currentDispatchIndex].get_st_time() + dispatchQ[currentDispatchIndex].get_pkt_on_air_time() + TL_radio_csma::txListenDelay;

			int NextTime = dispatchQ[currentDispatchIndex].get_st_time() + dispatchQ[currentDispatchIndex].get_pkt_on_air_time() + TL_radio_csma::txListenDelay;


			TL_SIM_dbg::dbg("TL_SIM_schedule", "[TL_SIM_schedule] step 2.3 perform receive at: %d", NextTime);

			// std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::receiveMessage, 
			// 								dispatchQ[currentDispatchIndex].get_dst(), 
			// 								dispatchQ[currentDispatchIndex].get_msgrece());

			// sim_event e(sim_scheduler::currentTime + TL_radio_csma::txListenDelay, dispatchQ[currentDispatchIndex].get_rx_node_id(), dispatchQ[currentDispatchIndex].get_recv_handler());
			sim_event e(NextTime, dispatchQ[currentDispatchIndex].get_rx_node_id(), dispatchQ[currentDispatchIndex].get_recv_handler());
			// sim_event e(NextTime, dispatchQ[currentDispatchIndex].get_rx_node_id(), s);


			ost << "TL_NB_UE::receiveMessage@[dst:" << dispatchQ[currentDispatchIndex].get_rx_node_id() << "]";
			e.set_handler_name(ost.str());
			sim_scheduler::sim_queue_insert(e);

			TL_SIM_dbg::dbg("TL_SIM_schedule", " [TL_SIM_schedule] %d tx success, to dst:%d", 
				dispatchQ[currentDispatchIndex].get_tx_node_id(), 
				dispatchQ[currentDispatchIndex].get_rx_node_id());
		}


		// 3. remove the current event from dispatchQ. Since this event is ready for checking, so it is safe to remove this event only.
		vector<sim_dispatch_event>::iterator itor;
		for (itor = dispatchQ.begin(); itor!= dispatchQ.end(); itor++)
		{
			if(*itor == one_dispatch){
				itor = dispatchQ.erase(itor);
				break;
			}
		}

		// TL_SIM_dbg::dbg("TL_NB_UE", "Node:%d 's neighbor: ,from %d.  isSink:%d", dst.get_moteid(), this->get_moteid(), isSink);
		// for(int i = 0; i < neighborList.size(); i++)
		// {
		// 	TL_SIM_dbg::dbg("TL_NB_UE", "%d", neighborList[i]);
		// }


	}


	int dispatch_endTime = get_local_time();
	sim_scheduler::timeConsumeDispatchQ += (dispatch_endTime - dispatch_startTime);




}


// @ [ISSUE] currently, there is only one pended event support.
void sim_scheduler::sim_queue_pend_event(int moteid)
{

	// for(int i=0 ;i< q.size(); i++)
	// {
	// 	if( q[i].get_mote() == moteid && q[i].get_priority() == sim_scheduler::LOW_PRO)
	// 	{
	// 		pendedQ.push_back(q[i]);
	// 		q.erase(q[i]); 
	// 		return;
	// 	}
		
	// }

	for(std::vector<sim_event>::iterator iter = q.begin()-1; iter != q.end(); iter++ )
	{
		if( iter->get_mote() == moteid && iter->get_priority() == sim_event_util::LOW_PRIORITY)
		{
			sim_event pend_event(iter->get_time(), iter->get_mote(), iter->get_handler());
			pendedQ.push_back(pend_event);
			q.erase(iter); 
			return;
		}
	}
} 


void sim_scheduler::sim_print_queue()
{
	cout << "==== [SHOW rest queue events] ===="<< endl;
	for(std::vector<sim_event>::iterator iter = q.begin(); iter != q.end(); iter++ )
	{
		cout << "[NODE " << iter->get_mote()  << "], at time :" << iter->get_time() << ", "<< iter->get_handler_name() << endl;
		// std::cout << " time " << iter->get_time() << ", at node " << iter->get_mote() << std::endl;
	}
	cout << "==== [queue events end] ===="<< endl;

}


long int sim_scheduler::get_local_time(){
	struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void sim_scheduler::sim_print_dispatchQ()
{
	int count=0;

	
	cout << "==== [SHOW dispatchQ] ===="<< endl;
	for(std::vector<sim_dispatch_event>::iterator iter = dispatchQ.begin(); iter != dispatchQ.end(); iter++ )
	{
		cout <<  "[Nm " << count <<"]" "[tx " << iter->get_tx_node_id()  << "], [rx " << iter->get_rx_node_id() << "], st_time:" <<  iter->get_st_time() << ", pkt_on_air_time:" << iter->get_pkt_on_air_time()  << ", collisionLabel:" << iter->get_predetermined_handler()    <<", msg:"<< iter->get_msgretx().get_payload() << endl;
		count++;

	}
	cout << "==== [end dispatchQ] ===="<< endl;
}

//print out the event content with the max number of maxC
void sim_scheduler::sim_print_queue_maxCount(int maxC)
{
	int currC = 0;
	if (maxC >= q.size()){
		maxC = q.size();
	}
	cout << "==== [SHOW rest queue events] ===="<< endl;
	for(std::vector<sim_event>::iterator iter = q.begin(); iter != q.end(); iter++ )
	{
		if (currC >= maxC)
		{
			break;
		}
		cout << "[NODE " << iter->get_mote()  << "], at time :" << iter->get_time() << 
			", execute:{"<< iter->get_handler_name() << "}" <<endl;
		currC++;
	}
	cout << "==== [queue events end] ===="<< endl;

}

void sim_scheduler::sim_print_pendQ()
{
	cout << "==== [SHOW pended  events] ===="<< endl;
	if(pendedQ.size() == 0)
	{
		cout << "[none]" << endl;
		return;
	}
	for(std::vector<sim_event>::iterator iter = pendedQ.begin(); iter != pendedQ.end(); iter++ )
	{
		cout << "[NODE " << iter->get_mote()  << "], at time :" << iter->get_time() << endl;
		// std::cout << " time " << iter->get_time() << ", at node " << iter->get_mote() << std::endl;
	}
}

