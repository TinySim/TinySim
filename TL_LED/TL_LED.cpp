#include "TL_LED.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_power_util.h"
#include "TL_SIM_schedule.h"

TL_LED::TL_LED(){
	this->current_state = "OFF";
	this->nodeId = 0;
	TL_SIM_dbg::dbg_add_channel("_Module_LED");
}
void TL_LED::toggle(){
	if(this->current_state == "ON"){
		this->turnOff();
	}
	else if(this->current_state == "OFF"){
		this->turnOn();
	}
}
void TL_LED::turnOn(){
	if(this->current_state == "ON"){
		// 当前LED处于运行状态
		return;
	}
	TL_SIM_dbg::dbg("_Module_LED", "[node %d] LED is ON", this->nodeId);
	this->current_state = "ON";
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::LED_ON].c_str());
}

void TL_LED::turnOff(){
	if(this->current_state == "OFF"){
		// 当前LED处于运行状态
		return;
	}
	TL_SIM_dbg::dbg("_Module_LED", "[node %d] LED is OFF", this->nodeId);
	this->current_state = "OFF";
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::LED_OFF].c_str());
}

string TL_LED::state(){
	return this->current_state ;
}

void TL_LED::setNodeId(int node){
	this->nodeId = node;
}
