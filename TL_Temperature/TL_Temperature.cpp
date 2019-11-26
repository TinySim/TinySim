#include <stdlib.h>
#include "TL_Temperature.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_power_util.h"
#include "TL_SIM_schedule.h"

TL_Temperature::TL_Temperature(){
	this-> current_temperature = 0.0;
	this-> nodeId = 0;
	TL_SIM_dbg::dbg_add_channel("_Module_Temperature");
}
void TL_Temperature::read(){
	 // srand((int)time(0));
	 int x = sim_scheduler::currentTime % 51;
	 x -= 10; // [-10,40]
	 this->current_temperature = (double) x;
	 TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::Temperature_Work].c_str());
}

double TL_Temperature::data(){
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime + 20, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::Temperature_Idle].c_str());
	// 此处假定温度传感器每次工作20ms的虚拟时间
	return this->current_temperature;
}

void TL_Temperature::setNodeId(int nodeID){
	this->nodeId = nodeID;
}
int TL_Temperature::getNodeId(){
	return this->nodeId ;
}