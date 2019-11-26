#include <stdlib.h>
#include "TL_Light.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_power_util.h"
#include "TL_SIM_schedule.h"

TL_Light::TL_Light(){
	this->nodeId = 0;
	this-> current_intensity = 0.0;
	TL_SIM_dbg::dbg_add_channel("_Module_Light");
}

void TL_Light::read(){
	// srand((int)time(0));
	int x = sim_scheduler::currentTime % 1001; // [0-1000]
	// x -= 10; // [-10,40]
	this->current_intensity = (double) x;
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::Light_Work].c_str());
}

void TL_Light::setData(double intensity){
	this->current_intensity = intensity;
}

double TL_Light::data(){
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime + 20, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::Light_Idle].c_str());
	// 此处假定光照传感器每次工作20ms的虚拟时间
	return this->current_intensity;
}

void TL_Light::setNodeId(int nodeID){
	this->nodeId = nodeID;
}
int TL_Light::getNodeId(){
	return this->nodeId ;
}