#ifndef SIMEVENT_H_INCLUDED
#define SIMEVENT_H_INCLUDED



#define MQTTCLIENT_QOS2 1

#include <memory.h>

#include <cstdio>
#define DEFAULT_STACK_SIZE -1
#include <iostream>
#include "TL_SIM_schedule.h"
#include "UE.h"
#include <chrono>
#include <thread>
#include <future>
#include <vector>
#include "sim_event_util.h"
#include "TL_Serial.h"
#include "TL_MQTTClient.h"
// #include "mqtt_linux.h"

#include "TL_GPS.h"
#include "TL_LOCK.h"
#include "TL_Time.h"
#include "TL_LED.h"
#include "TL_Temperature.h"
#include "TL_Light.h"
#include "TL_FAN.h"
#include "TL_SIM_Events.h"
#include "TL_SIM_Climate.h"

#include <typeinfo>

using namespace std;

class node{

public:

	node(TL_NB_UE& i_ue, TL_NB_UE& i_bs)
	{
		this->_inner_node_id = i_ue.get_moteid();
	}

	~node(){}

	void setup();

	void loop();

	void send(string msg) {
		socket_msg = msg;
		memset(socket_send, '\0', sizeof(char) * 100);
		for(int i=0; i<socket_msg.size(); i++)
		{
    	    socket_send[i] = socket_msg[i];
		}
		write(socket_fd, socket_send, 30);
		cout << "[send]" << socket_send << endl;
	}

	void receive() {
		memset(socket_receive, '\0', sizeof(char)*100);
		int size = read(socket_fd, socket_receive, sizeof(socket_receive));
		if(size > 0)
		{
    	    cout << "[receive]" << socket_receive << endl;
		}
	}

	void loop_end()
	{
		std::shared_future<void> s1 = async(launch::deferred, &node::loop_start, this);
		sim_event e1(sim_scheduler::currentTime + 00, get_nodeid(), s1);
		// printf("loop_end\n");
		sim_scheduler::sim_queue_insert(e1);
	}
	void loop_start()
	{
		std::shared_future<void> s1 = async(launch::deferred, &node::loop, this);
		sim_event e1(sim_scheduler::currentTime + 00, get_nodeid(), s1); 
		// printf("loop_start\n");
		sim_scheduler::sim_queue_insert(e1);
	}

	int get_nodeid()
	{
		return _inner_node_id;
	}
	// segment_for_time_module
private:
	// segment_for_sleep_time

	int _inner_node_id;
	int _inner_listenTime = 100;
	int _inner_nextWakeTime;
	int _inner_roundSendInterval = 20;	// 20ms per send
	int _inner_roundSendTime = 500;		// round send 500 ms in total.

	TL_Time tl_time;
	TL_Serial tl_serial;
	TL_LED tl_led;
	TL_Temperature tl_temperature;
	TL_NB tl_nb;
	TL_FAN tl_fan;
	TL_Light tl_light;
	TL_LOCK tl_lock;
	TL_GPS tl_gps;

	int socket_fd;
	string socket_msg;
	char socket_send[100];
	char socket_receive[100];

	// ####
	// segment_for_user_init
	// ####
};
#endif

