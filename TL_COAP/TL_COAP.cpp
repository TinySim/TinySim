#include "TL_COAP.h"
#include "UE.h"
#include "TL_SIM_power_util.h"
#include "TL_radio_trace.h"
#include "TL_radio.h"

#include <time.h>


using namespace std;
TL_COAP::TL_COAP(){
    this->ueId = 0;
    this->bsId = 0;
}

void TL_COAP::send(int node_id,double lat,double lng){
    // TL_SIM_dbg::dbg("node", " node [%d] wakup and execute GPSupload !", node_id);
    char position[50];
    sprintf(position,"%f%c%f%c%s",lat,'#',lng,'#',this->getTimeStamp().c_str());
    my_defined_coap_send(ip,port,"/other/gpsinfo",3, position,this->result_info, &interval, &delay);
}

void TL_COAP::send(int node_id,string str){
    // TL_SIM_dbg::dbg("node", " node [%d] wakup and execute GPSupload !", node_id);
    char position[50];
    sprintf(position,"%s%c%s",str.c_str(),'#',this->getTimeStamp().c_str());
    my_defined_coap_send(ip,port,"/other/gpsinfo",3, position,this->result_info, &interval, &delay);
}


void TL_COAP::sendIdByPut(int node_id,int close){
    if(close == 0)
        TL_SIM_dbg::dbg("_Module_Mobile", " Virtual Mobile Device is asking for opening LOCK of [node %d]",node_id);
    else if(close == 1)
        TL_SIM_dbg::dbg("node", " node [%d] wakup and execute lock !", node_id);

    char id[20];
    sprintf(id,"%d",node_id);

    if(close == 1){  //close
        char url_path[60] = "/other/lockrequest?close=1&ct="; // ct = current_time 
        strcat(url_path,this->getTimeStamp().c_str());
        int len = strlen(url_path);
        url_path[len] = '\0';
        my_defined_coap_send(ip,port,url_path, 3, id,this->result_info, &interval, &delay);
    } 
    else if(close == 0){
        char url_path[60] = "/other/lockrequest?close=0&ct=";
        strcat(url_path,this->getTimeStamp().c_str());
        int len = strlen(url_path);
        url_path[len] = '\0';
        my_defined_coap_send(ip,port,url_path, 3, id,this->result_info, &interval, &delay);
    }
}

void TL_COAP::roundSend(int node_id){
    TL_SIM_dbg::dbg("node", " node [%d] wakup and execute roundSend !", node_id);
    TL_SIM_dbg::dbg("roundSend", " Phone are executing unlock request of id %d!",node_id);
    char id_info[30];
    sprintf(id_info,"%d%c%s%s",node_id,'&',"ct=",getTimeStamp().c_str());
    my_defined_coap_send(ip,port,NULL,1,id_info,this->result_info, &interval, &delay);
    //cout<< node_id <<" result:"<<this->result_info << endl;
}

void TL_COAP::sendData(string url){
    char path[256];
    // url = "/receiveData?" + url;
    strcpy(path,url.c_str());

    // cout << "url in coap is : " << url << endl;
    // cout << "path in coap is: " << path << endl;

    // coap 模块处于发送状态
    TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", 
        sim_scheduler::currentTime, this->ueId, TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Send].c_str());

    // 如果coap下层是NB的话,那么此时NB也要处于活跃状态
    TL_NB_UE* _ue = TL_radio_trace::findUEById(this->ueId);
    TL_NB_UE* _bs = TL_radio_trace::findUEById(this->bsId);

    TL_SIM_dbg::dbg("TL_COAP", "ueID %d, bsID:%d", _ue->get_moteid(), _bs->get_moteid()); 


    if("NB" == this->macType ){
        if(_ue != NULL && _bs != NULL){
            TL_NB::Message outMessage(path, this->ueId, this->bsId);
            _ue->sendMessage(outMessage, *_bs);
            TL_SIM_dbg::dbg("TL_COAP", "[TL_COAP] af _ue->sendMessage"); 

        }
        TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
        sim_scheduler::currentTime, this->ueId, TL_SIM_power_util::state_name[TL_SIM_power_util::S_SLEEP].c_str());
    }
    else if("WiFi" == this->macType){
        // 需要改变对应的硬件状态
    }

    TL_SIM_dbg::dbg("TL_COAP", "[TL_COAP] before coap_send_data_by_get. %s, %s, %s, %s ", ip,port,path,this->result_info); 

    coap_send_data_by_get(ip,port,path,this->result_info);
    // coap 下行收到数据也需要处理,真实传输的数据就是result_info

    TL_SIM_dbg::dbg("TL_COAP", "[TL_COAP] coap_send_data_by_get. %s, %s, %s, %s ", ip,port,path,this->result_info); 


    // 如果coap下层是NB的话,那么此时NB也要处于活跃状态
    if("NB" == this->macType ){
        if(_ue != NULL && _bs != NULL){
            TL_SIM_dbg::dbg("TL_COAP", "[TL_COAP] _bs->sendMessage"); 

            TL_NB::Message inMessage(this->result_info, this->bsId, this->ueId);
            _bs->sendMessage(inMessage, *_ue);
        }
        TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", \
        sim_scheduler::currentTime, this->ueId, TL_SIM_power_util::state_name[TL_SIM_power_util::S_SLEEP].c_str());
    }
    else if("WiFi" == this->macType ){
        // 需要改变对应的硬件状态
    }
    // 此时一个发送动作完成，才让coap模块状态改变
    // coap 模块处于发送状态
    // 此处固定一个coap为20ms-40ms
    TL_SIM_dbg::dbg(TL_SIM_power_util::POWER_CHANNEL, "TIME:%d, NODE:%d, STATE:%s.", 
        sim_scheduler::currentTime + getRandomDelay(), this->ueId, TL_SIM_power_util::state_name[TL_SIM_power_util::COAP_Sleep].c_str());
}



char* TL_COAP::getResult(){
    return this->result_info;
}


void TL_COAP::connect(string s_ip,string s_port){
    strcpy(this->ip,s_ip.c_str());
    strcpy(this->port,s_port.c_str());
}

void TL_COAP::setMacType(string mactype){
    this->macType = mactype;
}

string TL_COAP::getMacType(){
    return this->macType;
}

string TL_COAP::getTimeStamp(){
     struct timeval tv;
     gettimeofday(&tv,NULL);
     struct tm* pTime;
     pTime = localtime(&tv.tv_sec);
     char sTemp[10] = {0};
     snprintf(sTemp, sizeof(sTemp), "%02d%03d", pTime->tm_sec, \
            tv.tv_usec / 1000);
     return (string)sTemp;
}

int TL_COAP::getDelay(){
    return this->delay;
}

int TL_COAP::getInterval(){
    return this->interval;
}

char* TL_COAP::getIP(){
    return ip;
}

void TL_COAP::setUEId(int id){
    this->ueId = id;
}

void TL_COAP::setBSId(int id){
    this->bsId = id;
}

void TL_COAP::setBSIdByUEid(int ueId){
    // find and set bs ID based on the topology
    for(int i = 0; i < TL_radio::linkList.size(); i++)
	{
		if(TL_radio::linkList[i].get_ueSrc().get_moteid() == ueId)
		{
			int tempNeighborId = TL_radio::linkList[i].get_ueDst().get_moteid();
			this->bsId = tempNeighborId;
            return;
		}

	}

}

int TL_COAP::getUEId(){
    return this->ueId;
}

int TL_COAP::getBSId(){
    return this->bsId;
}

// 获取一个coap send 的随机
// add by qiufj at 2019-07-17 20:05
int getRandomDelay(){
    srand((int)time(0));
    int delay = random() % 21;
    delay += 20; // 20 - 40ms的一个随机延迟
    return delay;
}