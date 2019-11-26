#ifndef SIMEVENT_H_INCLUDED
#define SIMEVENT_H_INCLUDED



#define MQTTCLIENT_QOS2 1

#include <memory.h>

#include <cstdio>

#include "MQTTClient.h"
// #include "mqtt_linux.h"

#define DEFAULT_STACK_SIZE -1

#include <iostream>
#include <sstream>

#include "TL_SIM_schedule.h"
#include "BaseStation.h"
#include "UE.h"
#include "TL_NB.h"
#include "TL_SIM_dbg.h"
#include <chrono>
#include <thread>
#include <future>

#include "TL_radio_trace.h"
#include "sim_event_util.h"

//#include "TL_COAP.h"  included in TL_NB.h

#include "TL_GPS.h"
#include "TL_LOCK.h"
#include "TL_Time.h"
#include "TL_LED.h"
#include "TL_Temperature.h"
#include "TL_Light.h"
#include "TL_FAN.h"

#include <typeinfo>

using namespace std;

class node{

public:

	node(TL_NB_UE& i_ue, TL_NB_UE& i_bs): _inner_ue(i_ue), _inner_bs(i_bs)
	{
		this->_inner_node_id = i_ue.get_moteid();
	}

	~node(){}

	void setup();

	void loop();

	void GPSupload();

	void messageArrived(string message);

	void roundQuery(int node_id,void(node::* func)(string));

	void loop_end()
	{
		_inner_ue.goToSleep();

		if(sim_scheduler::currentTime < get_nextWakeupTime())
		{
			std::shared_future<void> s1 = async(launch::deferred, &node::loop_start, this);
			sim_event e1(get_nextWakeupTime(), get_nodeid(), s1);
			sim_scheduler::sim_queue_insert(e1);

			ostringstream ost;
			ost << "node::loop_start";
			e1.set_handler_name(ost.str());
			set_nextWakeupTime(get_nextWakeupTime() + _inner_SleepTime);
		}
		else
		{	
			set_nextWakeupTime(get_nextWakeupTime() + _inner_SleepTime);
			loop_start();
		}

	}
	void loop_start()
	{
		std::shared_future<void> s1 = async(launch::deferred, &node::loop, this);
		sim_event e1(sim_scheduler::currentTime + 100, _inner_ue.get_moteid(), s1); 
		e1.set_priority(sim_event_util::LOW_PRIORITY);// pending the evnet that can be changed.

		ostringstream ost;
		ost << "node::loop";
		e1.set_handler_name(ost.str());

		sim_scheduler::sim_queue_insert(e1);
	}

	int get_cellid()
	{
		return _inner_bs.get_moteid();
	}

	int get_nodeid()
	{
		return _inner_node_id;
	}

	int get_sleepTime()
	{
		return _inner_SleepTime;
	}

	void set_sleepTime(int time)
	{
		_inner_SleepTime = time;
		int ueIndex = TL_radio_trace::getUEListIndex(_inner_ue.get_moteid());
		TL_radio_trace::ueList[ueIndex].set_SleepTime(time);
	}

	void set_nextWakeupTime(int time)
	{
		_inner_nextWakeTime = time;
		int ueIndex = TL_radio_trace::getUEListIndex(_inner_ue.get_moteid());
		TL_radio_trace::ueList[ueIndex].set_nextWakeupTime(time);
	}

	int get_nextWakeupTime()
	{
		return _inner_nextWakeTime;
	}


	void init_node()
	{
		int ueIndex = TL_radio_trace::getUEListIndex(_inner_ue.get_moteid());
		TL_radio_trace::ueList[ueIndex].set_state(TL_SIM_power_util::S_IDLE);
		
	}
	// segment_for_time_module

private:

	// segment_for_sleep_time

	int _inner_node_id;
	int _inner_listenTime = 100;
	int _inner_nextWakeTime;
	int _inner_roundSendInterval = 20;	// 20ms per send
	int _inner_roundSendTime = 500;		// round send 500 ms in total.
	

	TL_NB_UE _inner_ue;
	TL_NB_UE _inner_bs;

	TL_LOCK tl_lock;
	TL_GPS tl_gps;
	TL_NB tl_nb;
	TL_Time tl_time;
	TL_LED tl_led;
	TL_Temperature tl_temperature;
	TL_FAN tl_fan;
	TL_Light tl_light;


	// segment_for_user_init

};











#endif

