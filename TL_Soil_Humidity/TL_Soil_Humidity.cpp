#include <stdlib.h>
#include "TL_Soil_Humidity.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_power_util.h"
#include "TL_SIM_schedule.h"

TL_Soil_Humidity::TL_Soil_Humidity(){
	this->nodeId = 0;
	this-> current_soil_humidity = 0.0;
	TL_SIM_dbg::dbg_add_channel("_Module_Soil_Humidity");
}
void TL_Soil_Humidity::read(){
	 // srand((int)time(0));
	 int x = rand() % 101; // [0-100]
	 // x -= 10; // [-10,40]
	 this->current_soil_humidity = (double) x;
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::Light_Work].c_str());
}

double TL_Soil_Humidity::data(){
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime + 20, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::Light_Idle].c_str());
	// 此处假定土壤湿度传感器每次工作20ms的虚拟时间
	return this->current_soil_humidity;
}

void TL_Soil_Humidity::setNodeId(int nodeID){
	this->nodeId = nodeID;
}
int TL_Soil_Humidity::getNodeId(){
	return this->nodeId ;
}