#include <stdlib.h>
#include "TL_FAN.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_power_util.h"
#include "TL_SIM_schedule.h"

TL_FAN::TL_FAN(){
	this-> current_state = "OFF";
	this-> nodeId = 0;
	TL_SIM_dbg::dbg_add_channel("_Module_FAN");
}

// string TL_FAN::turnOn(){
// 	return "ON";
// }

// string TL_FAN::turnOff(){
// 	return "OFF";
// }


void TL_FAN::turnOn(){
	if(this->current_state == "ON"){
		// 当前风扇处于运行状态
		return;
	}
	TL_SIM_dbg::dbg("_Module_FAN", "[node %d] LED is ON", this->nodeId);
	this->current_state = "ON";
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::FAN_ON].c_str());
}

void TL_FAN::turnOff(){
	if(this->current_state == "OFF"){
		// 当前风扇处于关闭状态
		return;
	}
	TL_SIM_dbg::dbg("_Module_FAN", "[node %d] LED is OFF", this->nodeId);
	this->current_state = "OFF";
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::FAN_OFF].c_str());
}


void TL_FAN::toggle(){
	if(this->current_state == "ON"){
		this->turnOff();
	}
	else if(this->current_state == "OFF"){
		this->turnOn();
	}
}

string TL_FAN::state(){
	return this->current_state;
}

void TL_FAN::setNodeId(int nodeID){
	this->nodeId = nodeID;
}

int TL_FAN::getNodeId( ){
	return this->nodeId ;
}