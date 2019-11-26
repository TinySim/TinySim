#include "CloudDataGen.h"
#include "TL_SIM_dbg.h"
#include "TL_SIM_power_util.h"
#include "TL_SIM_time_util.h"
#include "TL_SIM_schedule.h"

CloudDataGen::CloudDataGen(){
	this->event_num = 0;
	this->nodenum = 0;
	this->startNode = 0;
	this->rand_seed = this->getRandByTime();
}
CloudDataGen::CloudDataGen(int nodenum,int startNode){
	this->event_num = 0;
	this->nodenum = nodenum;
	this->startNode = startNode;
	this->rand_seed = this->getRandByTime();
}

int CloudDataGen::getMobileId()
{

	return this->Mobile_Id;
}


void CloudDataGen::setEventNum(int num){
	this->event_num = num;
	coap.connect("127.0.0.1","5683");
}

int CloudDataGen::getRandBySelfSeed(){
	srand(this->rand_seed);
	return rand() % this->nodenum + this->startNode;
}

int CloudDataGen::getRandByTime(){
	srand(int(time(NULL)));
	return rand() % 2000;
}

int CloudDataGen::getRandLockTIme(){
	srand(this->rand_seed);
	return rand() % 60;
}

void CloudDataGen::executeRequestUnlock(string event){
	int minute =  60 * 1000; // 60 000ms
	int interval =  minute / this->event_num;
	for (int i = 0 ; i< this->event_num ; i++ ){
		int rand_id = this->getRandBySelfSeed();
		//cout<<"current rand_id is : " <<rand_id <<endl;
		std::shared_future<void> s1 = async(launch::deferred, &TL_COAP::sendIdByPut, &coap, rand_id, 0);
		// 之前只能发送固定的内容，我们现在不区分具体的函数，所有的数据发送都使用同一个函数,发送的内容全都存在字符串里面
		// 具体的事件处理逻辑交给用户自己
		// std::shared_future<void> s1 = async(launch::deferred, &TL_COAP::sendData, &coap, event);
		sim_event e1(sim_scheduler::currentTime + interval* i, rand_id, s1);

		ostringstream ost;
		ost << "TL_COAP::sendIdByPut@[" << rand_id  <<"]";
		e1.set_handler_name(ost.str());
		sim_scheduler::sim_queue_insert(e1);
		this->rand_seed++;
		if(this->rand_seed >= 2141483647){
			this->rand_seed = this->getRandByTime();
		}
	}
	// else if(executingTimes == true){
	// 	int id = getRandNum() % this->nodenum + this->startNode;
	// 	int interval = getRandNum() % 201 + 100;
	// 	std::shared_future<void> s1 = async(launch::deferred, &CloudDataGen::loop, this, id, interval, interval);
	// 	sim_event e1(sim_scheduler::currentTime + interval, id, s1);
	// 	sim_scheduler::sim_queue_insert(e1);
	// }
}

void CloudDataGen::executeEvent(string event){
	// 如果事件为空，即为不需要产生模拟手机时间
	if(event.empty() || event.size() == 0 || event == ""){
		return;
	}

	int mobileId = this->getMobileId();
	int minute =  60 * 1000; // 60 000ms
	int interval =  minute / this->event_num;
	string dummy = "" ;
	for (int i = 0 ; i< this->event_num ; i++ ){
		int rand_id = this->getRandBySelfSeed();
		dummy = event + "OPEN";
		//cout<<"current rand_id is : " <<rand_id <<endl;
		// std::shared_future<void> s1 = async(launch::deferred, &TL_COAP::sendIdByPut, &coap, rand_id, 0);
		// 之前只能发送固定的内容，我们现在不区分具体的函数，所有的数据发送都使用同一个函数,发送的内容全都存在字符串里面
		// 具体的事件处理逻辑交给用户自己
		dummy += "&nodeId=" + to_string(rand_id) ;
		std::shared_future<void> s1 = async(launch::deferred, &TL_COAP::sendData, &coap, dummy);
		sim_event e1(sim_scheduler::currentTime + interval* i, mobileId, s1);

		ostringstream ost;
		ost << "TL_COAP::sendData@[" << dummy << "]";
		e1.set_handler_name(ost.str());
		sim_scheduler::sim_queue_insert(e1);

		// 随机时间后上锁
		dummy = event + "CLOSE";
		dummy += "&nodeId=" + to_string(rand_id);
		std::shared_future<void> s2 = async(launch::deferred, &TL_COAP::sendData, &coap, dummy);
		sim_event e2(sim_scheduler::currentTime + interval* i + getRandLockTIme() * 1000, mobileId, s2);

		ost.str("");
		ost << "TL_COAP::sendData@[" << dummy << "]";
		e2.set_handler_name(ost.str());
		sim_scheduler::sim_queue_insert(e2);

		this->rand_seed++;
		if(this->rand_seed >= 2141483647){
			this->rand_seed = this->getRandByTime();
		}
	}
}


void CloudDataGen::loop(int node_id,int lastInterval, int lastTime){
	TL_SIM_dbg::dbg("_Module_Mobile","Virtual Mobile Device is asking for opening LOCK of [node %d]",node_id);		
	coap.sendIdByPut(node_id,0);
	string result = coap.getResult();
	if(result != "is_using"){
		//std::cout<<"CloudDataGen is called now-------------"<< coap.getResult()<<std::endl;
		TL_SIM_dbg::dbg(TL_SIM_time_util::MY_TIME_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
			sim_scheduler::currentTime - coap.getDelay(), node_id, TL_SIM_time_util::state_name[TL_SIM_time_util::Mobile_Send_Request].c_str());
	}else{
		//std::cout<<"[node]--------------> "<< node_id<<" is in using..."<< std::endl;
	}
	
	if(lastInterval > 300 )
		lastInterval -= 150;
	else if(lastInterval < 100)
		lastInterval += 150;

	int current_interval = rand()%lastInterval+50;
	int id = current_interval % this->nodenum + this->startNode;
	std::shared_future<void> s1 = async(launch::deferred, &CloudDataGen::loop, this, id, current_interval, lastTime+current_interval);
	sim_event e1(sim_scheduler::currentTime + current_interval + lastTime, id, s1);

		ostringstream ost;
		ost << "CloudDataGen::loop@[id:" << id << "]";
		e1.set_handler_name(ost.str());
	sim_scheduler::sim_queue_insert(e1);
}

