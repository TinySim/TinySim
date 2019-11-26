#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>
#include <thread>
#include <future>
#include <vector>

#include "json/json.h"
#include <cstdlib>

#include <sys/time.h>
#include <unistd.h>
#include "user_final_file.h"
#include "TL_radio_link.h"
#include "TL_radio.h"
#include "TL_radio_trace.h"
#include "TL_SIM_dbg.h"

#include "TL_SIM_powerp.h"

#include "TL_SIM_timep.h"

#include "bsDataGen.h"

#include "TL_SIM_mode.h"

#include "CloudDataGen.h"

using namespace std;

vector<node> nodeList;

int startBootTime = 100;

long int get_local_time(){
	struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
int strToint(string str){
	int count = 0;
	int length;
	length = str.length();
	if(length == 0)
		return count;
	for(int i=0; i<length; i++){
		count = 10 * count + str[i] - '0';
	}
	return count;
}

int main(int argc,char **argv) // paramater determine whether need config file 
{
	string topoFile = "../user/topo.txt";

	bool if_config  = false;

	if(argv[1] == "config"){
		if_config == true;
	}
	else{
		if_config = false;
	}
	// cout << (argv[1]=="config")<<endl;

	vector<string> channelList;
	vector<string> hardwareList;

	int nodenum = 1;				// default value
	string executeMode = "REAL";	// default value
	string executeTime = "0";		// default value
	// string V_Mobile = "no";			// default value no need virtual mobile events.

	if(1){
		string configFile = "../user/config.json";
		Json::Reader jsonReader;
		Json::Value jsonRoot;
		ifstream ifs_read_config;
		ifs_read_config.open(configFile,ios::binary);
		if(!ifs_read_config){
			cerr<<"can not find config file"<<endl;
			return 0;
		}
		if(!jsonReader.parse(ifs_read_config,jsonRoot)){
			// ofstream out("../result/application.log",ios::out);
			cout << "configuration File Format Error"<<endl;
			// out.close();
			// ofstream out1("../result/application_dummy.log",ios::out);
			// out1 << "configuration File Format Error";
			// out1.close();
			return 0;
		}
		if(jsonRoot.isMember("nodenum")){
			nodenum = jsonRoot["nodenum"].asInt();
		}
		if(jsonRoot.isMember("executeMode")){
			executeMode = jsonRoot["executeMode"].asString();
		}

		if(jsonRoot.isMember("executeTime")){
			executeTime = jsonRoot["executeTime"].asString();
		}
		// if(jsonRoot.isMember("V_Mobile")){
		// 	V_Mobile = jsonRoot["V_Mobile"].asString();
		// }
		if(jsonRoot.isMember("channel")){
			Json::Value channel_array = jsonRoot["channel"];
			for (unsigned int i=0; i < channel_array.size(); i++){
			    string channel = channel_array[i].asString();
			    channelList.push_back(channel); 
			}
		}
		if(jsonRoot.isMember("hardware")){
			 Json::Value hardware_array = jsonRoot["hardware"];
		     for (unsigned int i=0; i < hardware_array.size(); i++){
			     string hardware = hardware_array[i].asString();
			     hardwareList.push_back(hardware); 
		     } 
		}
	} // end if(if_config)
	cout << "配置文件加载完毕"<<endl;

	TL_radio::genTopoFromTrace(topoFile);

	cout<<"topo文件读取完毕"<<endl;

	// add watching channel for user code debugging.
	 for(int i=0; i < channelList.size() ; i++){
	 	TL_SIM_dbg::dbg_add_channel(channelList[i]);
	 }
	TL_SIM_mode::setNodeConfig(nodenum,1);

	for(int i = 0; i<hardwareList.size();i++){
		if("LOCK" == hardwareList[i])
			TL_SIM_mode::addOutput(TL_SIM_mode::LOCK);
		else if("GPS" == hardwareList[i])
			TL_SIM_mode::addOutput(TL_SIM_mode::GPS);
		else if("NB" == hardwareList[i])
			TL_SIM_mode::addOutput(TL_SIM_mode::NB);
		else if("LED" == hardwareList[i])
			TL_SIM_mode::addOutput(TL_SIM_mode::LED);
		else if("Temperature" == hardwareList[i])
			TL_SIM_mode::addOutput(TL_SIM_mode::Temperature);
		else if("FAN" == hardwareList[i])
			TL_SIM_mode::addOutput(TL_SIM_mode::FAN);
		else if("Light" == hardwareList[i])
			TL_SIM_mode::addOutput(TL_SIM_mode::Light);
	}

	cout << "硬件模块添加完毕"<<endl;

	if(executeMode == "VIRTUAL")
		TL_SIM_mode::setMode(TL_SIM_mode::VIRTUAL);
	else if(executeMode == "REAL")
		TL_SIM_mode::setMode(TL_SIM_mode::REAL);

	CloudDataGen *clouddata = new CloudDataGen(nodenum,1); 


	string _event = "mobileEvent?message=";

	// 真实模式下也可以使用tinysim内置的模拟手机功能，只要指定的事件不为空即可
	cout << "模拟移动设备端已经启动,事件开始产生"<< endl;
	clouddata->setEventNum(100);  // 10 per minute
	clouddata->executeEvent(_event);
	TL_SIM_dbg::dbg_add_channel("_Module_Mobile");





	// if(executeMode == "VIRTUAL"){
	// if(executeMode == "VIRTUAL" && V_Mobile == "yes"){
	// cout << "模拟移动设备端已经启动,事件开始产生"<< endl;
	// clouddata->setEventNum(10);  // 10 per minute
	// clouddata->executeEvent(_event);
	// TL_SIM_dbg::dbg_add_channel("_Module_Mobile");
	// }
	 // 10 per minute

	// initialize node and set boot time of each node.
	for(int i=0 ;i < TL_radio_trace::ueList.size() ;i++)
	{
		TL_radio_trace::ueList[i].set_bootTime(startBootTime * (i+1)); // first need set boot time for each node.
		TL_SIM_powerp::add_node_power_watch(TL_radio_trace::ueList[i]);// add for watching power consumption.
		TL_SIM_timep::add_node_time_watch(TL_radio_trace::ueList[i]);
		if(!TL_radio_trace::ueTobs.count(TL_radio_trace::ueList[i]))
			continue;
		node ntmp(TL_radio_trace::ueList[i], TL_radio_trace::ueTobs[TL_radio_trace::ueList[i]]);
		nodeList.push_back(ntmp);
	}

	// append the boot event of each node one-by-one
	for(int i=0 ; i< nodeList.size() ;i++)
	{
		std::shared_future<void> s = async(launch::deferred, &node::setup, &nodeList[i]);
		sim_event e(sim_scheduler::currentTime + TL_radio_trace::ueList[i].get_bootTime(), nodeList[i].get_nodeid(), s);
		ostringstream ost;
		ost << "node::setup";
		e.set_handler_name(ost.str());
		sim_scheduler::sim_queue_insert(e);
	}
	

	long int start_time = get_local_time();

	sim_event current_event, next_event;

	current_event = sim_scheduler::sim_queue_pop();

	// long int int_execute_time = strToint(executeTime) * 1000; // ms

	sim_scheduler::max_execute_time = strToint(executeTime) * 1000; 

	int t = 1; // mark to generate virtual mobile device requests events.
	
	for(int i=0 ; ; i++) // 
	 {
		// cout << "[SIMMAIN] " << current_event.get_time()  << ", at node " << current_event.get_mote() << endl;
	 	
	 	if(current_event.get_time() >= sim_scheduler::max_execute_time){
			//cout << "simulation task has been ended by TinySim"<<endl;
			// ofstream cout("../result/application.log",ios::out);
			// cout<< "simulation task has been ended by TinySim"<<endl;
			// cout.close();


			// ofstream ofs1("../result/application_all.log",ios::app);
			// cout<< "simulation task has been ended by TinySim"<<endl;
			// ofs1.close();
			break;
		}
		if (current_event.get_time() >  60 * 1000 * t ){

	 		// clouddata->executeRequestUnlock();
	 		clouddata->executeEvent(_event);
	 		t++;
	 	}



		current_event.get_handler().wait();

		next_event = sim_scheduler::sim_queue_pop();
		if(next_event.get_mote() == -1){// check from the pop events.
			break;
		}
		long int current_time = get_local_time();

		//cout <<"programs real time is :-------- " << current_time - start_time << "  ms" <<endl;
		if(executeMode == "REAL"){

			if( next_event.get_time() - current_event.get_time() >= 10 
								&& current_event.get_time() - (current_time - start_time) -1 > 0 )
				usleep((current_event.get_time() - ( current_time - start_time) - 1 ) * 1000  );
		}
		current_event = next_event;

		// TL_SIM_dbg::dbg("TL_SIM_main", "[TL_SIM_main] execute events. Node:%d, time: %d, Handl:%s", 
		// 	current_event.get_mote(), current_event.get_time(), current_event.get_handler_name().c_str());

		// cout << "[TL_SIM_main] execute events:" << current_event.get_handler_name().c_str() << endl;


		// sim_scheduler::sim_print_queue_maxCount(5);
		// sim_scheduler::sim_print_queue();
	}

	// ofstream out3("../result/application.log",ios::out);
	// cout<<"**The complete log can be downloaded by clicking the button 'Download Log'** "<<endl;
	cout<<"** [Finished in " <<(double(get_local_time() - start_time) / 1000 ) <<" s] **"<<endl;
	cout << "[+] The number of executed events: " << sim_scheduler::executedEventsCount << endl;
	cout << "[+] The number of retx events: " << sim_scheduler::retxEventsCout << endl;

	// cout.close();

	// ofstream out4("../result/application_all.log",ios::app);
	// cout<<"**The complete log can be downloaded by clicking the button 'Download Log'** "<<endl;
	// cout<<"** [Finished in " <<(double(get_local_time() - start_time) / 1000 ) <<" s] **"<<endl;
	// out4.close();

	TL_SIM_powerp::print_node_state();
	TL_SIM_powerp::print_state_time_consume_all();
	TL_SIM_powerp::print_state_power_consume_all();

	// ofstream out5("../result/runtime.log",ios::out);
	// out5 << "user#finish";  // user # finish or not
	// out5.close();

	// ofstream out6("../result/runtime_dummy.log",ios::out);
	// out6 << "user#finish";  // user # finish or not
	// out6.close();

	return 0;
}

