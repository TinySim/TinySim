
#include "TL_NB.h"
#include "UE.h"
#include "TL_radio_csma.h"

#include "TL_radio_link.h"
#include "TL_radio.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_power_util.h"
#include "TL_SIM_powerp.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <sstream>

#include "bsDataGen.h"
#include "TL_radio_trace.h"
#include "MQTTClient.h"

// #include <future>

using namespace std;

double pktEr;

void TL_NB_UE::messageArrived(MQTT::Message& md)
{
}

void TL_NB_UE::goToSleep()
{
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.",
					sim_scheduler::currentTime, this->get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_SLEEP].c_str());
}

void TL_NB_UE::receiveMessage(TL_NB::Message msg)
{
		
	// TL_radio_link link = TL_radio::findLink(msg.get_moteid(), this->get_moteid());
	// double ber = link.get_erRate();
	// int msgLen = msg.get_payload().size()*8;
	// double pktEr =  1-pow((1-ber), msgLen);

	TL_SIM_dbg::dbg("TL_NB_UE", " [TL_NB_UE] in receive. TIME: %d, bs %d receive:[%s] from node %d, txCount:%d, errRate:%f",
			sim_scheduler::currentTime, msg.get_dstid(),  msg.get_payload().c_str(), msg.get_srcid(), msg.get_txNum(), pktEr);

	if (isSink)
	{
		// printf("TIME:%d \t [dst] Receive data %s, from node %d.\n", sim_scheduler::currentTime, msg.get_payload(), msg.get_moteid());
		// cout << "TIME:" << sim_scheduler::currentTime  <<  "\t [dst] Receive data " << msg.get_payload() << ", from node " << msg.get_moteid() << endl;
		// cout << "TIME:" << sim_scheduler::currentTime  <<  "\t [dst] Receive data " \
		//  	<< msg.get_payload() << ", from node " << msg.get_moteid() \
		//  	<< ", txCount:" << currentTxTime
		//  	<< ", errRate:" << pktEr
		//  	<< endl;



		TL_SIM_dbg::dbg("node", " TIME: %d, bs %d receive:[%s] from node %d, txCount:%d, errRate:%f",
						sim_scheduler::currentTime, msg.get_dstid(),  msg.get_payload().c_str(), msg.get_srcid(), msg.get_txNum(), pktEr);
		// currentTxTime = 0;

		// TL_SIM_dbg::dbg(TL_SIM_time_util::TIME_CHANNEL, "TIME:%d, NODE:%d, STATE:%s, TXNUM:%d.", \
		//  sim_scheduler::currentTime, msg.get_moteid(),\
		//  TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_RECEIVED].c_str(), \
		//  msg.get_txNum());

		TL_SIM_dbg::dbg(TL_SIM_time_util::TIME_CHANNEL, "TIME:%d, SRCNODE:%d, DSTNODE:%d, STATE:%s, TXNUM:%d.",
						sim_scheduler::currentTime, msg.get_srcid(), msg.get_dstid(),
						TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_RECEIVED].c_str(),
						msg.get_txNum());
		// TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.",
		// 		sim_scheduler::currentTime, this->get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_RECEIVE].c_str());

	}
	else // node receive msg from bs following here.
	{
		TL_SIM_dbg::dbg("node", " sink:%d, src:%d, dst and here:%d",  this->get_isSink(), msg.get_srcid(), msg.get_dstid());


		// printf("TIME:%d \t [node] Receive data %s, from node %d.\n", sim_scheduler::currentTime, msg.get_payload(), msg.get_moteid());
		// cout << "TIME:" << sim_scheduler::currentTime  <<  "\t [Node] Receive data " << msg.get_payload() << ", from node " << msg.get_moteid() << endl;
		TL_SIM_dbg::dbg("node", " TIME: %d, node %d receive:[%s] from bs %d, txCount:%d, errRate:%f",
						sim_scheduler::currentTime, msg.get_dstid(), msg.get_payload().c_str(), msg.get_srcid(), msg.get_txNum(), pktEr);

		// TL_SIM_dbg::dbg(TL_SIM_time_util::TIME_CHANNEL, "TIME:%d, NODE:%d, STATE:%s, TXNUM:%d.", \
		//  sim_scheduler::currentTime, msg.get_moteid(), \
		//  TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_RECEIVED].c_str(), \

		sim_scheduler::retxEventsCout += (msg.get_txNum()-1); // obtain the number of retx events from here.


		TL_SIM_dbg::dbg(TL_SIM_time_util::TIME_CHANNEL, "TIME:%d, SRCNODE:%d, DSTNODE:%d, STATE:%s, TXNUM:%d.",
						sim_scheduler::currentTime, msg.get_srcid(), msg.get_dstid(),
						TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_RECEIVED].c_str(),
						msg.get_txNum()
						);
		// TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.",
		// 				sim_scheduler::currentTime, this->get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_RECEIVE].c_str());

		// insert event is done, and then upgrade the pend event.

		//sim_scheduler::sim_queue_upgrade_pend_event_to_queue(msg.get_srcid(), sim_scheduler::currentTime);


		//cout << "[bsDataGen], tx ok then generate new events." << endl;

		//bsDataGen::BSEventGen();

		// jmp to the user code of mqtt msg.
		MQTT::Message message;
		message.qos = MQTT::QOS0;
		message.retained = false;
		message.dup = false;
		message.payload = (void *)(msg.get_payload().c_str());
		message.payloadlen = msg.get_payload().size() + 1;
		this->messageArrived(message);
	}

	// we do not decide whether the node should go sleep or not. The sleep thing is left for develop to define.
	//@ event instrumentation.
	// std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::goToSleep, this);
	// sim_event e(sim_scheduler::currentTime + TL_radio_csma::txListenDelay, this.get_moteid(), s);
	// sim_scheduler::sim_queue_insert(e);

	// @ we just give a listen delay waiting for others msg.
	// sim_scheduler::currentTime += TL_radio_csma::txListenDelay;
}

void TL_NB_UE::retxMessage(TL_NB::Message msg, TL_NB_UE dst)
{
	// sim_scheduler::currentTime += TL_radio_csma::txListenDelay;
	// currentTxTime += 1;
	if (msg.get_txNum() > maxTxTime)
	{
		// cout << "[ Error exceeds max tx times]. for msg" << msg.get_payload() << ", and node "<< msg.get_moteid() << endl;
		TL_SIM_dbg::dbg("node", " TIME: %d, node %d fail received data from node %d, txCount:%d, errRate:%f",
						sim_scheduler::currentTime, dst.get_moteid(), msg.get_srcid(), msg.get_txNum(), pktEr);
		// currentTxTime = 0;

		sim_scheduler::retxEventsCout += (maxTxTime-1);// increase the retx message with the maximum retransmission time.

		// if not tx msg failed, then sleep sometime.
		//@ event instrumentation.
		// std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::goToSleep, this);
		// sim_event e(sim_scheduler::currentTime + TL_radio_csma::txListenDelay, this.get_moteid(), s);
		// sim_scheduler::sim_queue_insert(e);

		// @ we just give a listen delay waiting for others msg. nop for specific listening function.
		// sim_scheduler::currentTime += TL_radio_csma::txListenDelay;

		if (isSink)
		{
			// sim_event e();
			// e.set_time(sim_scheduler::currentTime);
			// e.set_mote();
			// sim_scheduler::sim_queue_upgrade_pend_event_to_queue(dst.get_moteid(), sim_scheduler::currentTime);
			// cout << "[bsRetx], upgrade "

			// @ this is keeping on trying retex next time
			// int ueIndex = TL_radio_trace::getUEListIndex(dst.get_moteid());
			// int nextWake = TL_radio_trace::ueList[ueIndex].get_nextWakeupTime();
			// int sleepTime = TL_radio_trace::ueList[ueIndex].get_sleepTime();
			// std::shared_future<void> s1 = async(launch::deferred, &TL_NB_UE::sendMessage, this ,msg, dst);
			// sim_event e1( nextWake + 2 , TL_radio_trace::bs.get_moteid(), s1);
			// sim_scheduler::sim_queue_insert(e1);
			// TL_SIM_dbg::dbg("node", " keeping on trying next time:%d, from %d to %d.", nextWake + 2 ,\
			// 	this->get_moteid(), dst.get_moteid());

			// @ this is just randomly select other nodes.
			//bsDataGen::BSEventGen();
		}

		// fail to receive also is to record.
		TL_SIM_dbg::dbg(TL_SIM_time_util::TIME_CHANNEL, "TIME:%d, SRCNODE:%d, DSTNODE:%d, STATE:%s, TXNUM:%d.",
						sim_scheduler::currentTime, msg.get_srcid(), msg.get_dstid(),
						TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_RECEIVED].c_str(),
						msg.get_txNum());

		return;
	}


// ---------------------------new collssion---------------------------beg

	msg.set_txNum(msg.get_txNum() + 1);

	int msgLenBits = msg.get_payload().size() * 8;
	// int pkt_on_air_time = int((msgLenBits*1.0/TL_NB::NB_IOT_bitrate) * 1000); // ms

	int pkt_on_air_time = 102; // modify this time anyway to create collisions,ms


	std::shared_future<void> sRetx = async(launch::deferred, &TL_NB_UE::retxMessage, this, (msg), (dst));
	std::shared_future<void> sReceive = async(launch::deferred, &TL_NB_UE::receiveMessage, &dst, (msg));

	TL_SIM_dbg::dbg("TL_NB_UE", "[TL_NB_UE] %p, %p",  sRetx, sReceive);


	sim_scheduler::rx_dispatcher(sim_scheduler::currentTime, pkt_on_air_time, this->get_moteid(),  dst.get_moteid(), 
		sRetx,  msg,  &dst, this,
		sReceive, msg);
// ---------------------------new collssion---------------------------end


// ---------------------------previous backup---------------------------beg
	// TL_radio_link* link = TL_radio::findLink(this->get_moteid(), dst.get_moteid());

	// double ber = 0.0;
	// msg.set_txNum(msg.get_txNum() + 1);
	// if(link != NULL)
	// 	double ber = link->get_erRate();
	// else{
	// 	ber = 0.0;
	// }
	// // cout << "[DBG], into  sendmsg, test1\n";
	// // cout <<  msg.get_payload() << "," << msg.get_moteid() << endl;
	// int msgLen = msg.get_payload().size() * 8;

	// // cout << "[DBG], into  sendmsg, test2\n";
	// pktEr = 1 - pow((1 - ber), msgLen);

	// double errorPro = rand() / double(RAND_MAX);
	// ostringstream ost;


	// // TODO, put rx into the dispatch.
	


	// if (errorPro < pktEr)
	// {
	// 	// retxMessage( msg, dst);

	// 	//@ event instrumentation.
	// 	/* [cgl] when packets are erroreous, the node then backoff and retx.*/


	// 	// std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::set_noise, this,  2);
	// 	// cout << "[DBG], into  sendmsg, to retx.\n";
	// 	// std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::retxMessage, this,  std::ref(msg) , std::ref(dst));
	// 	std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::retxMessage, this, (msg), (dst));

	// 	int backoffTime = TL_radio_csma::slotTime + (TL_radio_csma::maxBackOffTime-TL_radio_csma::slotTime)*rand();
	// 	sim_event e(sim_scheduler::currentTime + backoffTime, this->get_moteid(), s);
	// 	// sim_event e(sim_scheduler::currentTime + TL_radio_csma::txListenDelay, this->get_moteid(), s);
	// 	ost << "TL_NB_UE::retxMessage@[dst:" << dst.get_moteid() << "]";
	// 	e.set_handler_name(ost.str());
	// 	sim_scheduler::sim_queue_insert(e);

	// 	TL_SIM_dbg::dbg("node", " %d retx, to dst:%d", this->get_moteid(), dst.get_moteid());

	// 	// 	cout << "[debug_ue] retxMessage:"
	// 	// << typeid(&TL_NB_UE::retxMessage).name()
	// 	// << ", thisUE:" << typeid(this).name()
	// 	// << endl;
	// }
	// else
	// {
	// 	// cout << "[DBG], into  sendmsg, received.\n";
	// 	// dst.receiveMessage(msg);

	// 	//@ event instrumentation.
	// 	// std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::receiveMessage, &dst, std::ref(msg));
	// 	std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::receiveMessage, &dst, (msg));

	// 	sim_event e(sim_scheduler::currentTime + TL_radio_csma::txListenDelay, dst.get_moteid(), s);
	// 	ost << "TL_NB_UE::receiveMessage@[dst:" << dst.get_moteid() << "]";
	// 	e.set_handler_name(ost.str());
	// 	sim_scheduler::sim_queue_insert(e);

	// 	TL_SIM_dbg::dbg("node", " %d tx success, to dst:%d", this->get_moteid(), dst.get_moteid());
	// }


// ---------------------------previous backup---------------------------end


	// sim_scheduler::sim_print_queue_maxCount(5);
	// sim_scheduler::sim_print_queue();



	sim_event et = sim_scheduler::sim_queue_pop();
	// cout << "[READYEXECUTE], TIME:" << sim_scheduler::currentTime << ", node:" << this->get_moteid() << ", txCount:" <<  currentTxTime  << endl;

	if(et.get_mote() == -1){// it is time to goback.
		return;
	}

	TL_SIM_dbg::dbg("TL_NB_UE", "[TL_NB_UE] execute events. Node:%d, time: %d, Handl:%s", et.get_mote(), et.get_time(), et.get_handler_name().c_str());

	// cout << "[TL_NB_UE] execute events:" << et.get_handler_name().c_str() << endl;


	// cout << "[NODE " << iter->get_mote()  << "], at time :" << iter->get_time() << ", "<< iter->get_handler_name() << endl;

	et.get_handler().wait();

	TL_SIM_dbg::dbg("TL_NB_UE", "[TL_NB_UE] Over.Event. Node:%d, time: %d, Handl:%s ", et.get_mote(), et.get_time(), et.get_handler_name().c_str());


	// cout << "[sentx Event execute.]" << endl;
}

void TL_NB_UE::sendMessage(TL_NB::Message msg, TL_NB_UE dst)
{

		if (isSink)
		{

			TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.",
							sim_scheduler::currentTime , dst.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_RECEIVE].c_str());
		}
		else{
			TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.",
						sim_scheduler::currentTime, this->get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_SEND].c_str());

			std::shared_future<void> s = async(launch::deferred, &TL_NB_UE::goToListen, this);

			// sim_event e(sim_scheduler::currentTime + 10, dst.get_moteid(), s);
			sim_event e(sim_scheduler::currentTime + 10, this->get_moteid(),  s);

			ostringstream ost;
			ost << "TL_NB_UE::goToListen";
			e.set_handler_name(ost.str());
			sim_scheduler::sim_queue_insert(e);			
		}

		TL_SIM_dbg::dbg(TL_SIM_time_util::TIME_CHANNEL, "TIME:%d, SRCNODE:%d, DSTNODE:%d, STATE:%s, TXNUM:%d.",
						sim_scheduler::currentTime, this->get_moteid(), dst.get_moteid(),
						TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_SEND].c_str(),
						msg.get_txNum());

		retxMessage(msg,dst);


}

void TL_NB_UE::goToListen(){
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.",
				sim_scheduler::currentTime, this->get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_LISTEN].c_str());

	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.",
				sim_scheduler::currentTime + 10, this->get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_SLEEP].c_str());
}
void TL_NB_UE::join(int cellid)
{
}

void TL_NB_UE::set_isSink(bool sinkSet)
{
	this->isSink = sinkSet;
}

int TL_NB_UE::get_isSink()
{
	return this->isSink;
}

void TL_NB_UE::set_xdmen(int i_xdmen)
{
	this->xdmen = i_xdmen;
}

void TL_NB_UE::set_ydmen(int i_ydmen)
{
	this->ydmen = i_ydmen;
}

void TL_NB_UE::set_moteid(int moteid)
{
	this->m_moteid = moteid;
}

void TL_NB_UE::set_txPower(int i_txPower)
{
	this->txPower = i_txPower;
}

void TL_NB_UE::set_noise(int i_noise)
{
	this->noise = i_noise;
}

int TL_NB_UE::get_txPower()
{
	return txPower;
}

int TL_NB_UE::get_noise()
{
	return noise;
}

int TL_NB_UE::get_xdmen()
{
	return xdmen;
}

int TL_NB_UE::get_ydmen()
{
	return ydmen;
}

int TL_NB_UE::get_maxTxTime()
{
	return maxTxTime;
}

int TL_NB_UE::get_currentTxTime()
{
	return currentTxTime;
}

int TL_NB_UE::get_moteid() const
{
	return m_moteid;
}

void TL_NB_UE::set_state(int i_state)
{
	this->state = i_state;
}

int TL_NB_UE::get_state()
{
	return this->state;
}

void TL_NB_UE::set_SleepTime(int i_sleepTime)
{
	this->sleepTime = i_sleepTime;
}
int TL_NB_UE::get_SleepTime()
{
	return this->sleepTime;
}

int TL_NB_UE::get_bootTime()
{
	return this->bootTime;
}
void TL_NB_UE::set_bootTime(int i_bootTime)
{
	this->bootTime = i_bootTime;
}

void TL_NB_UE::set_nextWakeupTime(int time)
{
	nextWakeTime = time;
}

int TL_NB_UE::get_nextWakeupTime()
{
	return nextWakeTime;
}

/*
* refresh by qiufujian at 2018-8-23
*/
double TL_NB_UE::get_coverRange(){
	return this->coverRange;
}
void TL_NB_UE::set_coverRange(double Range){
	this->coverRange = Range;
}

int TL_NB_UE::get_carryCapacity(){
  	return this->carryCapacity;
}
void TL_NB_UE::set_carryCapacity(int carry){
	this->carryCapacity = carry;
}

int TL_NB_UE::get_currentCarry(){
	return this->currentCarry ;
}
void TL_NB_UE::set_currentCarry(int carry){
	this->currentCarry = carry;
}
