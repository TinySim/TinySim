// #include "TL_LOCK.h"
#include <chrono>
#include <thread>
#include <future>

#include "TL_SIM_schedule.h"
#include "BaseStation.h"
#include "TL_SIM_dbg.h"
#include "sim_event_util.h"
#include "TL_SIM_power_util.h"

#include "user_final_file.h"

TL_LOCK::TL_LOCK(){
	this->nodeId = 0;
	this->lock_state = "Locked";
	TL_SIM_dbg::dbg_add_channel("_Module_LOCK");
}

// void TL_LOCK::lock(TL_NB_UE ue, TL_NB_UE bs,TL_COAP coap){
//     if(this->lock_state == 1){

//     	//cout << "[node] "<<ue.get_moteid() << " are executing locking......"<<endl;
// 		TL_SIM_dbg::dbg("_Module_LOCK", "[node %d] LOCK is CLOSE", ue.get_moteid()); // 2018-10-18 10:07
// 		TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 			sim_scheduler::currentTime, ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::LOCK_LOCKing].c_str());
		
// 		this->lock_state = 0;
        
// 		TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 			sim_scheduler::currentTime+100, ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::LOCK_Idle].c_str());

// 		TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 			sim_scheduler::currentTime, ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Send].c_str());
		
// 		if("NB" == coap.getMacType()){
// 			TL_NB::Message outMessage("example data!", ue.get_moteid(), bs.get_moteid());
// 			ue.sendMessage(outMessage, bs);

// 			TL_NB::Message inMessage("example data!",bs.get_moteid(), ue.get_moteid());
// 			bs.sendMessage(inMessage, ue);

// 			TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 			sim_scheduler::currentTime, ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::S_SLEEP].c_str());
// 		}

// 		coap.sendIdByPut(ue.get_moteid(),1);

// 		TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 			sim_scheduler::currentTime + coap.getDelay(), ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Sleep].c_str());
// 	}
// }

// void TL_LOCK::unlock(TL_NB_UE ue, TL_NB_UE bs, TL_COAP coap){
// 	if(this->lock_state == 0){

// 	//cout << "[node] "<<ue.get_moteid() << " are executing unlocking......"<<endl;
// 	TL_SIM_dbg::dbg("_Module_LOCK", "[node %d] LOCK is OPEN,costing %d ms.", ue.get_moteid(), coap.getInterval());// 2018-10-18 10:08

// 	TL_SIM_dbg::dbg(TL_SIM_time_util::MY_TIME_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 			sim_scheduler::currentTime, ue.get_moteid(), TL_SIM_time_util::state_name[TL_SIM_time_util::LOCK_OPEN].c_str());

// 	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 	sim_scheduler::currentTime, ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::LOCK_UNLOCKing].c_str());

// 	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
// 	sim_scheduler::currentTime+100, ue.get_moteid(), TL_SIM_power_util::state_name[TL_SIM_power_util::LOCK_Idle].c_str());
		
// 	this->lock_state = 1;

// 	std::shared_future<void> s1 = async(launch::deferred, &TL_LOCK::lock, this, ue, bs, coap);
// 	sim_event e1(sim_scheduler::currentTime + sim_scheduler::currentTime % 1000 + 10000, ue.get_moteid(), s1);  // lock in 10000+ ms after unlock
// 	sim_scheduler::sim_queue_insert(e1);
// }

// 2019-07-17 20:50
// 开锁关锁这个动作都会持续100ms
void TL_LOCK::lock(){
	if(this->lock_state == "Locked"){
		// 当前LOCK处于锁住状态
		TL_SIM_dbg::dbg("_Module_LOCK", "[node %d] LOCK is already CLOSED", this->nodeId);
		return;
	}
	TL_SIM_dbg::dbg("_Module_LOCK", "[node %d] LOCK is CLOSE", this->nodeId); // 2018-10-18 10:07
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
		sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::LOCK_LOCKing].c_str());

	this->lock_state = "Locked";

	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
		sim_scheduler::currentTime+100, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::LOCK_Idle].c_str());
}

void TL_LOCK::unlock(){
	if(this->lock_state == "UnLocked"){
		// 当前LOCK处于打开状态
		TL_SIM_dbg::dbg("_Module_LOCK", "[node %d] can not open LOCK since it is already OPENED", this->nodeId);
		return;
	}
	TL_SIM_dbg::dbg("_Module_LOCK", "[node %d] LOCK is OPEN", this->nodeId); // 2018-10-18 10:07
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
		sim_scheduler::currentTime, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::LOCK_UNLOCKing].c_str());

	this->lock_state = "UnLocked";
	
	TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
		sim_scheduler::currentTime+100, this->nodeId, TL_SIM_power_util::state_name[TL_SIM_power_util::LOCK_Idle].c_str());

	// std::shared_future<void> s1 = async(launch::deferred, &TL_LOCK::lock, this);
	// sim_event e1(sim_scheduler::currentTime + sim_scheduler::currentTime % 1000 + 10000, this->nodeId, s1);  // lock in 10000+ ms after unlock
	// sim_scheduler::sim_queue_insert(e1);
}

void TL_LOCK::setNodeId(int id){
	this->nodeId = id;
}

int TL_LOCK::getNodeId(){
	return this->nodeId;
}

string TL_LOCK::state(){
	return this->lock_state;
}