#define MQTTCLIENT_QOS2 1

#include <memory.h>
#include "MQTTClient.h"
#define DEFAULT_STACK_SIZE -1
#include <iostream>
#include "TL_SIM_schedule.h"
#include "BaseStation.h"
#include "UE.h"

#include "TL_SIM_dbg.h"
#include <chrono>
#include <thread>
#include <future>

#include "TL_radio_trace.h"
#include "sim_event_util.h"
#include "user_final_file.h"

#include <typeinfo>

using namespace std;

void node::messageArrived(string message){
    // segment_for_callback_function
}

void node::roundQuery(int node_id,void(node::* func)(string))
{
	// TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	// 		sim_scheduler::currentTime, _inner_ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Send].c_str());
	
	// if("NB" == _inner_coap.getMacType()){
	// 	TL_NB::Message outMessage("example data!", get_nodeid(), _inner_bs.get_moteid());
	// 	_inner_ue.sendMessage(outMessage, _inner_bs);
		
	// 	TL_NB::Message inMessage("example data!",_inner_bs.get_moteid(), get_nodeid());
	// 	_inner_bs.sendMessage(inMessage, _inner_ue);

	//     TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	// 	sim_scheduler::currentTime, _inner_ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_SLEEP].c_str());
	// }
	// else if("WiFi" == _inner_coap.getMacType()){
	// }
	// _inner_coap.roundSend(get_nodeid());
	// TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	// 	sim_scheduler::currentTime + _inner_coap.getDelay(), _inner_ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Sleep].c_str());

	// 去服务器查询关于该节点的消息
	string queryData = "queryData?nodeId="+ to_string(get_nodeid());
	_inner_coap.sendData(queryData);
	string result = _inner_coap.getResult();
	// cout << "coap server result " << result << endl;
	(this->*func)(result);
}


// segment_for_time_module


void node::setup() {

	//TL_SIM_dbg::dbg("node", " node [%d] setup!", get_nodeid());
	init_node();
	set_sleepTime(_inner_SleepTime);
	set_nextWakeupTime(_inner_ue.get_bootTime() + _inner_SleepTime);
	// segment_for_user_setup




	// 将各个硬件模块与节点进行绑定
	tl_lock.setNodeId(_inner_node_id);
	tl_gps.setNodeId(_inner_node_id);
	_inner_coap.setUEId(_inner_node_id);
	_inner_coap.setBSIdByUEid(_inner_node_id);
	tl_led.setNodeId(_inner_node_id);
	tl_temperature.setNodeId(_inner_node_id);
	tl_fan.setNodeId(_inner_node_id);
	tl_light.setNodeId(_inner_node_id);	
	// end 绑定

	std::shared_future<void> s1 = async(launch::deferred, &node::loop_start, this);
	sim_event e1(sim_scheduler::currentTime + 100, _inner_ue.get_moteid(), s1);
		ostringstream ost;
		ost << "node::loop_start";
		e1.set_handler_name(ost.str());
	sim_scheduler::sim_queue_insert(e1);
}

void node::loop() {
	
	
	// TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	// 	sim_scheduler::currentTime , _inner_ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Send].c_str()); 
	//TL_SIM_dbg::dbg("node", " node [%d] wakup and execute loop !", get_nodeid());

	// roundQuery(get_nodeid(), &node::messageArrived);
	// segment_for_round_send


	// segment_for_user_loop

	// TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	// sim_scheduler::currentTime + _inner_coap.getDelay() , _inner_ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Sleep].c_str());
	 
	loop_end();

}





