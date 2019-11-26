#include "TL_GPS.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_schedule.h"
#include "TL_NB.h"
#include "TL_radio_trace.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>


// void TL_GPS::read(TL_NB_UE ue,TL_NB_UE bs, TL_COAP coap){

//     TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 	sim_scheduler::currentTime , ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::GPS_Tracking].c_str());

// 	gps.read();

// 	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 	sim_scheduler::currentTime + 50, ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::GPS_Acquisition].c_str());

// 	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 	sim_scheduler::currentTime + 100, ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::GPS_Sleep].c_str());

// 	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 		sim_scheduler::currentTime + coap.getDelay(), ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Send].c_str());
// }

TL_GPS::TL_GPS(){
	this->nodeId = 0;
	this->xDmen = 0;
	this->yDmen = 0;
}
void TL_GPS::read(){

    TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime , this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::GPS_Tracking].c_str());

	gps.read();

	// 需要返回节点的位置坐标
	TL_NB_UE* ue = TL_radio_trace::findUEById(this->nodeId);

	if(ue != NULL ){
		this->xDmen = ue->get_xdmen();
		this->yDmen = ue->get_ydmen();
	}

	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime + getRandomTimeConsume() , this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::GPS_Acquisition].c_str());

	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
	sim_scheduler::currentTime + getRandomTimeConsume() + getRandomTimeConsume() , this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::GPS_Sleep].c_str());

}
	
double TL_GPS::latData(){
	return gps.latData();
}
double TL_GPS::lngData(){
	return gps.lngData();
}

void TL_GPS::setNodeId(int id){
	this->nodeId = id;
}

int TL_GPS::getNodeId(){
	return this->nodeId;
}

int TL_GPS::get_xDmen(){
	return this->xDmen;
}

int TL_GPS::get_yDmen(){
	return this->yDmen;
}

// 获取一个coap send 的随机
// add by qiufj at 2019-07-17 20:32
int getRandomTimeConsume(){
    srand((int)time(0));
    int consume = random() % 31;
    consume += 20; // 20 - 50ms的一个随机延迟
    return consume;
}