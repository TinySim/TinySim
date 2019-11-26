#include "TL_SIM_powerp.h"
#include "TL_SIM_dbg.h"
#include "UE.h"
#include "TL_SIM_power_node.h"
#include "TL_SIM_power_util.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "TL_SIM_mode.h"

using namespace std;


// int TL_SIM_powerp::S_STATE_NUM = 4;
// int TL_SIM_powerp::S_IDLE = 0;
// int TL_SIM_powerp::S_SEND = 1;
// int TL_SIM_powerp::S_RECEIVE = 2;
// int TL_SIM_powerp::S_SLEEP = 3;

// string TL_SIM_powerp::state_name = {"idle", "send", "receive", "sleep"};
// double TL_SIM_powerp::state_power_consume_NB_BC95 = {1.04, 3.173, 3.173, 0.004727}; // mA
// string TL_SIM_powerp::POWER_CHANNEL = "POWER_CHANNEL";
// string TL_SIM_powerp::power_regex = R"(TIME:(.+), NODE:(.+), STATE:(.+)\.)";

// const static int TOTAL_STATE_NUM = 22;

vector<TL_SIM_power_node> TL_SIM_powerp::node_p_list;// the list used for recoding the power related features of all nodes

map<int , string> TL_SIM_powerp::state_map;

int TL_SIM_powerp::mark[22];

void TL_SIM_powerp::add_node_power_watch(TL_NB_UE& i_ue)
{
	setVarToMark(); // init mark array
	TL_SIM_power_node node_power;

	node_power.set_moteid(i_ue.get_moteid());

	if(i_ue.get_bootTime() == -1)
	{
		cout << "[ERROR] node needs first set boot time!. exit."<< endl;
		exit(1);
	}

	//可以在这里设置三个或者四个不同的set_module_pre_state,用于分割开不同模块之间的并行状态，这样就可以避开时间之间的冲突，以免互相影响

	//node_power.set_pre_time(i_ue.get_bootTime());
	//node_power.set_pre_state(i_ue.get_state());

	node_power.set_NB_pre_time(i_ue.get_bootTime());
	node_power.set_NB_pre_state(TL_SIM_power_util::S_SLEEP);

	node_power.set_LOCK_pre_time(i_ue.get_bootTime());
	node_power.set_LOCK_pre_state(TL_SIM_power_util::LOCK_Idle);

	node_power.set_GPS_pre_time(i_ue.get_bootTime());
	node_power.set_GPS_pre_state(TL_SIM_power_util::GPS_Sleep);

	node_power.set_COAP_pre_time(i_ue.get_bootTime());
	node_power.set_COAP_pre_state(TL_SIM_power_util::COAP_Sleep);

	node_power.set_LED_pre_time(i_ue.get_bootTime());
	node_power.set_LED_pre_state(TL_SIM_power_util::LED_OFF);

	// add by qiufj at 2019-07-16 19:09
	node_power.set_Temperature_pre_time(i_ue.get_bootTime());
	node_power.set_Temperature_pre_state(TL_SIM_power_util::Temperature_Idle);

	node_power.set_FAN_pre_time(i_ue.get_bootTime());
	node_power.set_FAN_pre_state(TL_SIM_power_util::FAN_OFF);


	node_power.set_Light_pre_time(i_ue.get_bootTime());
	node_power.set_Light_pre_state(TL_SIM_power_util::Light_Idle);


	node_p_list.push_back(node_power);

	//map<string,string> state_pair;
	
	//state_pair["GSP"] = "Idle";

	// state_map[i_ue.get_moteid()] = "Locked";


}


void TL_SIM_powerp::do_profile_power(string buf)
{

    vector<string> log_ele_list;
    TL_SIM_dbg::dbg_filter(buf, TL_SIM_power_util::power_regex, log_ele_list);

    int time = stoi(log_ele_list[1]);
    int node = stoi(log_ele_list[2]);
    int state;

	for(int var = 0; var < TL_SIM_power_util::total_state_num; var++){
    		if(log_ele_list[3] == TL_SIM_power_util::state_name[var]){
    			state  =  var ;
    		}
    }


    TL_SIM_power_node node_power;  

	if(get_power_node(node, node_power))
    {

    	if (0 <= state && state <= 5){

    		// if(node_power.get_NB_pre_state() == 1 || node_power.get_NB_pre_state() == 2 )
    		// cout << " [node] "<< node << "\tcurrent state :"<< TL_SIM_power_util::state_name[state] << "\tpre state " << TL_SIM_power_util::state_name[node_power.get_NB_pre_state()] 
    		//      << "\tcurrent time: "<< time << "\tpre time:"<< node_power.get_NB_pre_time()<<endl;

    		node_power.update_state_time_consume(node_power.get_NB_pre_state(), time - node_power.get_NB_pre_time());
    		node_power.set_NB_pre_time(time);
    		node_power.set_NB_pre_state(state);
    		
    	}

    	else if (6 <= state && state <= 8){
    		node_power.update_state_time_consume(node_power.get_LOCK_pre_state(), time - node_power.get_LOCK_pre_time());
    		node_power.set_LOCK_pre_time(time);
    		node_power.set_LOCK_pre_state(state);
    		if(state == 6 || state == 7){
    			node_power.set_LAST_LOCK_state(state);
    		}
    	}

    	else if (9 <= state && state <= 11){
    		node_power.update_state_time_consume(node_power.get_GPS_pre_state(), time - node_power.get_GPS_pre_time());
    		node_power.set_GPS_pre_time(time);
    		node_power.set_GPS_pre_state(state);
    	}
    	else if (12 <= state && state <= 13){

    		node_power.update_state_time_consume(node_power.get_COAP_pre_state(), time - node_power.get_COAP_pre_time());
    		node_power.set_COAP_pre_time(time);
    		node_power.set_COAP_pre_state(state);
    	}
    	else if (14 <= state && state <= 15){

    		node_power.update_state_time_consume(node_power.get_LED_pre_state(), time - node_power.get_LED_pre_time());
    		node_power.set_LED_pre_time(time);
    		node_power.set_LED_pre_state(state);
    	}
    	else if (16 <= state && state <= 17){
    		node_power.update_state_time_consume(node_power.get_Temperature_pre_state(), time - node_power.get_Temperature_pre_time());
    		node_power.set_Temperature_pre_time(time);
    		node_power.set_Temperature_pre_state(state);
    	}
    	else if (18 <= state && state <= 19){
    		node_power.update_state_time_consume(node_power.get_FAN_pre_state(), time - node_power.get_FAN_pre_time());
    		node_power.set_FAN_pre_time(time);
    		node_power.set_FAN_pre_state(state);
    	}
    	else if (20 <= state && state <= 21){
    		node_power.update_state_time_consume(node_power.get_Light_pre_state(), time - node_power.get_Light_pre_time());
    		node_power.set_Light_pre_time(time);
    		node_power.set_Light_pre_state(state);
    	}

    	// refresh at 2018-10-16
		  //   	if(state == 7 || state == 14){
		  //   		//print_node_state(TL_SIM_mode::stateList);
				// ;
		  //   	}
    	//if (state == )
    	//node_power.update_four_module_state();   
    	update_node_power(node_power);
    }
}


int TL_SIM_powerp::get_power_node_index(int moteid)
{
	for(int i=0 ;i < node_p_list.size(); i++)
	{
		if(node_p_list[i].get_moteid() == moteid)
		{
			return i;
		}
	}
	return -1;
}


void TL_SIM_powerp::print_node_state(){
	vector<int> nodeList;
	get_all_moteid(nodeList);


	// ofstream cout("../result/state.log",ios::out);
	
	if(TL_SIM_mode::stateList.size() == 0){
		return;
	}
	cout << "====== Show  all nodes state at "<< getCurrentTime() << "  ======" << endl;
	for(int i=0 ;i<nodeList.size() ;i++)
	{
		TL_SIM_power_node node_power;

		if(get_power_node(nodeList[i], node_power))
		{
			cout << "[node " << nodeList[i] << "] "<<endl;
			
			for( auto iter =TL_SIM_mode::stateList.begin();iter != TL_SIM_mode::stateList.end(); iter++ ){
				switch(*iter){
					case 1:
						cout<<" LOCK state: "<<TL_SIM_power_util::state_name[node_power.get_LAST_LOCK_state()];
						break;
					case 2:
						cout<<" GPS state: " <<TL_SIM_power_util::state_name[node_power.get_GPS_pre_state()];
						break;
					case 3:
						cout<<" COAP state: " <<TL_SIM_power_util::state_name[node_power.get_COAP_pre_state()];
						break;
					case 4:
						cout<<" NB state: " << TL_SIM_power_util::state_name[node_power.get_NB_pre_state()];
						break;
					case 5:
						cout<<" LED state: " << TL_SIM_power_util::state_name[node_power.get_LED_pre_state()];
						break;
					case 6:
						cout<<" Temperature state: " << TL_SIM_power_util::state_name[node_power.get_Temperature_pre_state()];
						break;
					case 7:
						cout<<" FAN state: " << TL_SIM_power_util::state_name[node_power.get_FAN_pre_state()];
						break;
					case 8:
						cout<<" Light state: " << TL_SIM_power_util::state_name[node_power.get_Light_pre_state()];
						break;
					default:
					    break;
				}
			}
			cout << endl;
		}

	}
	// cout.close();
}


// @ return: whether get power node successfully.
bool TL_SIM_powerp::get_power_node(int moteid, TL_SIM_power_node& node_power)
{
	for(int i=0 ;i < node_p_list.size(); i++)
	{
		if(node_p_list[i].get_moteid() == moteid)
		{
			node_power = node_p_list[i];
			return true; 
			// return node_p_list[i];
		}
	}
	return false;
	// return *(TL_SIM_power_node*)nullptr;
}

// TL_SIM_power_node& TL_SIM_powerp::get_power_node(int moteid, bool& isFind)
// {
// 	for(int i=0 ;i < node_p_list.size(); i++)
// 	{
// 		if(node_p_list[i].get_moteid() == moteid)
// 		{
// 			// node_power = node_p_list[i];
// 			isFind = true; 
// 			return node_p_list[i];
// 		}
// 	}
// 	// return false;
// 	return *(TL_SIM_power_node*)nullptr;
// }



void TL_SIM_powerp::print_state_time_consume_nodelist(const vector<int>& nodeList)
{	
	//cout<<endl;
	// ofstream cout("../result/delay.log",ios::out);
	ostringstream oss;
	cout << "====== Show nodes different state time consumption at " << getCurrentTime() <<"  ======" << endl;
	
	for(int i=0 ;i<nodeList.size() ;i++)
	{
		TL_SIM_power_node node_power_tmp;
		if(get_power_node(nodeList[i], node_power_tmp))
		{
			cout << "[node " << nodeList[i] << "]"<<endl;
			int count = 1;
			for(int j=0 ; j< TL_SIM_power_util::total_state_num ; j++)
			{
				if(mark[j] != 1 || j == 8) // LOCK idle
					continue;
				// cout << TL_SIM_power_util::state_name[j] << ":" << node_p_list[node_power_i].get_state_time_consume(j) << "\t";
				if(j == TL_SIM_power_util::S_SEND || j == TL_SIM_power_util::S_RECEIVE){
					oss<<TL_SIM_power_util::state_name[j] << ":" << node_power_tmp.get_state_time_consume(j) + node_power_tmp.get_state_time_consume(12) / 2 << " ms";
				}
				else{
					oss<< TL_SIM_power_util::state_name[j] << ":" << node_power_tmp.get_state_time_consume(j)  <<" ms";
				}
				cout<<setiosflags(ios::left)<<setw(40)<<oss.str();
				oss.str("");
				if(count % 4 == 0){
					cout<<endl;
				}
				count++;
			}
			cout << endl;
		}

	}
	// cout.close();
}

void TL_SIM_powerp::print_state_time_consume_all()
{
	vector<int> nodeList;
	get_all_moteid(nodeList);
	print_state_time_consume_nodelist(nodeList);
}


void TL_SIM_powerp::print_state_power_consume_nodelist(const vector<int>& nodeList)
{
	//cout << endl;
	ostringstream oss;
	// ofstream cout("../result/power.log",ios::out);
	cout << "====== Show nodes different state power consumption at "<< getCurrentTime() <<"  ======" << endl;
	for(int i=0 ;i<nodeList.size() ;i++)
	{
		TL_SIM_power_node node_power_tmp;
		// bool isFind;
		// node_power_tmp = get_power_node(nodeList[i], isFind);
		// if(isFind)
		if(get_power_node(nodeList[i], node_power_tmp))
	    // int node_power_i=get_power_node_index(nodeList[i]);
    	// if( node_power_i != -1)
		{
			cout <<"[node " << nodeList[i] << "]"<<endl;

			// for(int j=0 ; j< TL_SIM_power_util::S_STATE_NUM ; j++)
			// {
			// 	node_p_list[node_power_i].set_state_power_consume(j, node_p_list[node_power_i].get_state_time_consume(j) * TL_SIM_power_util::state_power_consume_NB_BC95[j]);
			// 	cout << TL_SIM_power_util::state_name[j] << ":" << node_p_list[node_power_i].get_state_power_consume(j) << "\t";

			// }
			// double sumPower = 0;
			// for(int j=0 ; j< TL_SIM_power_util::S_STATE_NUM ; j++){sumPower += node_p_list[node_power_i].get_state_power_consume(j);}
			// cout << "totalPower:" << sumPower << endl;
                       	int count = 1;
			for(int j=0 ; j< TL_SIM_power_util::total_state_num ; j++)
			{
				if(mark[j] != 1 || j == 8) // lock_idle
					continue; // this state is not exists!
				// double tmpP = node_power_tmp.get_state_time_consume(j) * TL_SIM_power_util::state_power_consume_NB_BC95[j];
				
				double voltage = 0.0;
				if( 0 <= j && j <= 5)
					voltage = TL_SIM_power_util::NB_BC95_V;
				else if ( 6 <= j && j <= 8)
					voltage = TL_SIM_power_util::LOCK_V;
				else if ( 9 <= j && j <= 11)
					voltage = TL_SIM_power_util::GPS_V;
				else if ( 12 <= j && j <= 13 )
					voltage = TL_SIM_power_util::COAP_V;
				else if ( 14 <= j && j <= 15 )
					voltage = TL_SIM_power_util::LED_V;
				else if ( 16 <= j && j <= 17 )
					voltage = TL_SIM_power_util::Temperature_V;
				else if ( 18 <= j && j <= 19 )
					voltage = TL_SIM_power_util::FAN_V;
				else if ( 20 <= j && j <= 21 )
					voltage = TL_SIM_power_util::Light_V;

				if( j == 1 || j== 2){
					node_power_tmp.update_state_time_consume(j, node_power_tmp.get_state_time_consume(12) / 2);
				}
				node_power_tmp.set_state_power_consume(j, \
					node_power_tmp.get_state_time_consume(j) * TL_SIM_power_util::state_power_consume[j] * voltage / 1000);
				oss << TL_SIM_power_util::state_name[j] << ":" << node_power_tmp.get_state_power_consume(j) << "mJ";
				cout<<setiosflags(ios::left)<<setw(40)<<oss.str();
				oss.str("");	
				if(count % 4 == 0){
					cout<<endl;
				}
				count++;
				
			}
			cout<<endl;
			double sumPower = 0;
			for(int j=0 ; j < TL_SIM_power_util::total_state_num ; j++)
				{
					if(mark[j] != 1)
						continue;
					sumPower += node_power_tmp.get_state_power_consume(j);
				}
			cout << "totalPower:" << sumPower << "mJ" << endl;
		}
		
	}
	// cout.close();
	cout<<">>";
}


void TL_SIM_powerp::print_state_power_consume_all()
{
	vector<int> nodeList;
	get_all_moteid(nodeList);
	print_state_power_consume_nodelist(nodeList);
}


void TL_SIM_powerp::get_all_moteid(vector<int>& nodeList)
{
	// vector<int> nodeList;
	for(int i=0 ;i< node_p_list.size() ;i++)
	{
		nodeList.push_back(node_p_list[i].get_moteid());
	}
	// return nodeList;
}


void TL_SIM_powerp::update_node_power(TL_SIM_power_node& node_power)
{
	for(int i=0 ;i < node_p_list.size(); i++)
	{
		if(node_p_list[i].get_moteid() == node_power.get_moteid())
		{
			node_p_list[i] = node_power;
			return;
			// node_power = node_p_list[i];
			// return true; 
			// return node_p_list[i];
		}
	}
}
void TL_SIM_powerp::setVarToMark(){
    for(int i=0;i<22;i++){  // array size equal to total_state_num
    	mark[i] = 0;
    }
    for( auto iter = TL_SIM_mode::stateList.begin();iter != TL_SIM_mode::stateList.end(); iter++ ){
    	switch (*iter){
    		case 1:   // LOCK
    			for(int i = 0 ;i<3;i++){
    				mark[6 + i] = 1;
    			}
    			break;
    		case 2:   // GPS
    			for(int i = 0; i<3;i++){
    				mark[9+i] = 1;
    			}
    			break;
    		case 3:	 // COAP
    			for(int i=0;i<2;i++){
    				mark[12+i] = 1;
    			}
    			break;
    		case 4: // NB
    			for(int i=0;i<6;i++){
    				mark[i] = 1;
    			}
    			break;
    		case 5:  // LED
    		    for(int i=0;i<2;i++){
    				mark[14+i] = 1;
    			}
    			break;
    		
    		case 6:  // Temperature
    		    for(int i=0;i<2;i++){
    				mark[16+i] = 1;
    			}
    			break;
    		case 7:  // FAN
    		    for(int i=0;i<2;i++){
    				mark[18+i] = 1;
    			}
    			break;
    		case 8:  // Light
    		    for(int i=0;i<2;i++){
    				mark[20+i] = 1;
    			}
    			break;
    		default:
    			break;
    	}
    }
}
string TL_SIM_powerp::getCurrentTime(){
	time_t now = time(0);// 基于当前系统的当前日期/时间  
    tm *ltm = localtime(&now);
    char result[60];  

    char iyear[10],imonth[10],iday[10],ihour[10],imin[10],isec[10];  
    sprintf(iyear, "%d",1900 + ltm->tm_year );  
    sprintf(imonth, "%02d", 1 + ltm->tm_mon );  
    sprintf(iday, "%02d", ltm->tm_mday );  
    sprintf(ihour, "%02d", ltm->tm_hour );  
    sprintf(imin, "%02d",  ltm->tm_min);  
    sprintf(isec, "%02d",  ltm->tm_sec);  

    memset(result , 0 , sizeof(result));
    strcat(result,iyear);
    strcat(result,"-");
    strcat(result,imonth);
    strcat(result,"-");
    strcat(result,iday);
    strcat(result," ");
    strcat(result,ihour);
    strcat(result, ":");
    strcat(result, imin);
    strcat(result, ":");
    strcat(result,isec);

    return (string)result;
}

