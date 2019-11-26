#include "TL_SIM_timep.h"
#include "TL_SIM_dbg.h"
#include "UE.h"
#include "TL_SIM_time_node.h"
#include "TL_SIM_time_util.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>



using namespace std;

vector<TL_SIM_time_node> TL_SIM_timep::node_time_list;// the list used for recoding the time related features of all nodes


void TL_SIM_timep::add_node_time_watch(TL_NB_UE& i_ue)
{
	TL_SIM_time_node node_time;
	node_time.set_moteid(i_ue.get_moteid());

	if(i_ue.get_bootTime() == -1)
	{
		cout << "[ERROR] node needs first set boot time!. exit."<< endl;
		exit(1);
	}

	node_time.set_pre_time(i_ue.get_bootTime());
	node_time.set_pre_state(i_ue.get_state());

	node_time.set_open_lock_pre_time(i_ue.get_bootTime());
	node_time.set_open_lock_pre_state(TL_SIM_time_util::Mobile_Send_Request);

	node_time_list.push_back(node_time);

}


void TL_SIM_timep::do_my_profile_time(string buf){

	vector<string> log_ele_list;
    TL_SIM_dbg::dbg_filter(buf, TL_SIM_time_util::my_time_regex, log_ele_list);

    int current_time = stoi(log_ele_list[1]);
    int node = stoi(log_ele_list[2]);
    int state;

	for(int var = 0; var < TL_SIM_time_util::S_STATE_NUM; var++){
    		if(log_ele_list[3] == TL_SIM_time_util::state_name[var]){
    			state  =  var ;
    		}
    }

    TL_SIM_time_node node_time;  

	if(get_time_node(node, node_time))
    {


        if(state == TL_SIM_time_util::Mobile_Send_Request){
           // cout<<"----------------now current_time point is "<<current_time <<endl;
            node_time.set_open_lock_pre_state(state);
            node_time.set_open_lock_pre_time(current_time);
        }
        else if(state == TL_SIM_time_util::LOCK_OPEN ){
            if(node_time.get_open_lock_pre_state() == TL_SIM_time_util::Mobile_Send_Request){
                    int delay = current_time - node_time.get_open_lock_pre_time();
                    // cout<<"[node] "<< node << " costs "<< delay << " ms to open lock!"<<endl;
            }
            node_time.set_open_lock_pre_state(state);
            node_time.set_open_lock_pre_time(current_time); 

        }

		//node_time.set_pre_state(state); // the state must update
		update_node_time(node_time);
    }
}

void TL_SIM_timep::do_profile_time(string buf)
{

	//cout << "== timedbg ==" << buf << endl;
	// cout << "[node]" << buf << endl;
    vector<string> log_ele_list;
    // TL_SIM_dbg::dbg_filter(buf, TL_SIM_time_util::time_regex, log_ele_list);
    TL_SIM_dbg::dbg_filter(buf, TL_SIM_time_util::time_regex_muti, log_ele_list);



    int time = stoi(log_ele_list[1]);
    int srcnode = stoi(log_ele_list[2]);
    int dstnode = stoi(log_ele_list[3]);
    int state;
    int txNum = stoi(log_ele_list[5]);

    if(log_ele_list[4] == TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_SEND])
    {
    	state = TL_SIM_time_util::S_PKT_SEND;
    }
    else if(log_ele_list[4] == TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_RECEIVED])
    {
    	state = TL_SIM_time_util::S_PKT_RECEIVED;
    }	
    else if(log_ele_list[4] == TL_SIM_time_util::state_name[TL_SIM_time_util::S_PKT_PEND])
    {
    	state = TL_SIM_time_util::S_PKT_PEND;
    }

    TL_SIM_time_node node_time;    
    // bool isFind;
    // get_power_node(node, node_time);

    // int node_time_i = get_power_node_index(node);
    // if( node_time_i != -1)
    // node_time = get_power_node(node, isFind);
    // if(isFind)
	if(get_time_node(srcnode, node_time))
    {


    	// check if need add new nodes and initialize.
    	if(!node_time.isKeyInMap(dstnode))
    	{
    		node_time.set_pkt_delay(dstnode, 0);
    		node_time.set_intend_pkt_num(dstnode, 0);
    		node_time.set_tx_pkt_num(dstnode, 0);
    	}


    	// cout << " == timedbg == [before]";
    	// cout << "[node]" << node_time.get_moteid() 
    	// 	<< ", [intendPkt]" <<  node_time.get_intend_pkt_num(dstnode)
    	// 	<< ", [TxPkt]" << node_time.get_tx_pkt_num(dstnode) << ", [totalDelay]" << node_time.get_pkt_delay(dstnode) 
    	// 	<< endl;


    	// cout << " == timedbg == [currnent]";
    	// cout << "[pre_state]" << node_time.get_pre_state() << ", [pre_time]" << node_time.get_pre_time() << ", [nowtime]" << time << endl;
    	if(state ==  TL_SIM_time_util::S_PKT_PEND)
    	{
    		node_time.set_pre_time(time);
    	}
    	else if ( state ==  TL_SIM_time_util::S_PKT_SEND && node_time.get_pre_state() == TL_SIM_time_util::S_PKT_PEND)
    	{
    		node_time.set_pkt_delay(dstnode ,node_time.get_pkt_delay(dstnode) + time - node_time.get_pre_time());
    		// node_time.set_pkt_delay(node_time.get_pkt_delay() + time - node_time.get_pre_time());
	    	node_time.set_pre_time(time);
    	}
    	else if ( state ==  TL_SIM_time_util::S_PKT_RECEIVED && node_time.get_pre_state() == TL_SIM_time_util::S_PKT_SEND)
    	{
    		// node_time.set_pkt_delay(node_time.get_pkt_delay() + time - node_time.get_pre_time());
    		// node_time.set_intend_pkt_num(node_time.get_intend_pkt_num() +1 );
    		// node_time.set_tx_pkt_num(node_time.get_tx_pkt_num() + txNum);

    		node_time.set_pkt_delay(dstnode, node_time.get_pkt_delay(dstnode) + time - node_time.get_pre_time());
    		node_time.set_intend_pkt_num(dstnode, node_time.get_intend_pkt_num(dstnode) +1 );
    		node_time.set_tx_pkt_num(dstnode, node_time.get_tx_pkt_num(dstnode) + txNum);

	    	node_time.set_pre_time(time);

    	}
    	// cout << " == timedbg == [after]";
    	// cout << "[node]" << node_time.get_moteid() 
    	// 	<< ", [intendPkt]" <<  node_time.get_intend_pkt_num(dstnode)
    	// 	<< ", [TxPkt]" << node_time.get_tx_pkt_num(dstnode) << ", [totalDelay]" << node_time.get_pkt_delay(dstnode) 
    	// 	<< endl;

		node_time.set_pre_state(state); // the state must update
		update_node_time(node_time);


    	// cout << "===maybe BUg here@" << endl;

    }

    // cout << "== powerdbg ==" << buf << endl;



    // for(int i=0 ;i < log_ele_list.size(); i++)
    // {
    // 	cout << "[1]->" << log_ele_list[i] << endl;
    // }
}

// int TL_SIM_timep::get_power_node_index(int moteid)
// {
// 	for(int i=0 ;i < node_time_list.size(); i++)
// 	{
// 		if(node_time_list[i].get_moteid() == moteid)
// 		{
// 			return i;
// 			// node_time = &node_time_list[i];
// 			// return true; 
// 			// return node_time_list[i];
// 		}
// 	}
// 	return -1;
// 	// return *(TL_SIM_power_node*)nullptr;
// }



// @ return: whether get power node successfully.
// bool TL_SIM_powerp::get_power_node(int moteid, TL_SIM_power_node* node_power)
// {
// 	for(int i=0 ;i < node_time_list.size(); i++)
// 	{
// 		if(node_time_list[i].get_moteid() == moteid)
// 		{
// 			node_power = &node_time_list[i];
// 			return true; 
// 			// return node_time_list[i];
// 		}
// 	}
// 	return false;
// 	// return *(TL_SIM_power_node*)nullptr;
// }



// @ return: whether get power node successfully.
bool TL_SIM_timep::get_time_node(int moteid, TL_SIM_time_node& node_time)
{
	for(int i=0 ;i < node_time_list.size(); i++)
	{
		if(node_time_list[i].get_moteid() == moteid)
		{
			node_time = node_time_list[i];
			return true; 
			// return node_time_list[i];
		}
	}
	return false;
	// return *(TL_SIM_power_node*)nullptr;
}


void TL_SIM_timep::print_ave_pkt_delay_nodelist(const vector<int>& nodeList)
{
    ofstream ofs("../result/other.log",ios::out);
	ofs << "====== Show nodes average PKT TX delay at "<<getCurrentTime()<<"  ======  simulation is running in background..." << endl;
	for(int i=0 ;i<nodeList.size() ;i++)
	{
		TL_SIM_time_node node_time_tmp;
		if(get_time_node(nodeList[i], node_time_tmp))
		{
			ofs << "[node " << nodeList[i] << "]" << endl;
			vector<int> dstNodeList;
			node_time_tmp.get_all_dst_moteid(dstNodeList);
			if(dstNodeList.size() == 0)
			{
				ofs << "\t--> NONE" << endl;
			}
			for(int i=0 ;i< dstNodeList.size(); i++)
			{
				// cout << "[node " << nodeList[i] << "--> "<<  dstNodeList[i] <<"] ";
				ofs << "--> "<<  dstNodeList[i] <<"] ";
				double pktDelay = node_time_tmp.get_pkt_delay(dstNodeList[i])*1.0/node_time_tmp.get_intend_pkt_num(dstNodeList[i]);
				double pdr = node_time_tmp.get_intend_pkt_num(dstNodeList[i])*1.0/node_time_tmp.get_tx_pkt_num(dstNodeList[i]);
				ofs << "avePKTDELAY:" << pktDelay << " ms\t";
				ofs << "PDR:" << pdr << "\t";
				ofs << "intendPkt:" << node_time_tmp.get_intend_pkt_num(dstNodeList[i]) << "\t";
				ofs << "txNum:" << node_time_tmp.get_tx_pkt_num(dstNodeList[i]) << "\t";
				ofs << endl;
                //cout <<">>";
			}

		    // for(map<int, int>::iterator it = linkTime.begin(); it != linkTime.end(); it++)
			// {
			// 	cout << "[node " << nodeList[i] << "--> "<<  it->first <<"] ";
			// 	double pktDelay = node_time_tmp.get_pkt_delay(it->first)*1.0/node_time_tmp.get_intend_pkt_num(it->first);
			// 	double pdr = node_time_tmp.get_intend_pkt_num(it->first)*1.0/node_time_tmp.get_tx_pkt_num(it->first);
			// 	cout << "avePKTDELAY:" << pktDelay << "ms\t";
			// 	cout << "PDR:" << pdr << "\t";
			// 	cout << "intendPkt:" << node_time_tmp.get_intend_pkt_num(it->first) << "\t";
			// 	cout << "txNum:" << node_time_tmp.get_tx_pkt_num(it->first) << "\t";
			// 	cout << endl;
			// }

			// cout << "[node " << nodeList[i] << "] ";
			// double pktDelay = node_time_tmp.get_pkt_delay()*1.0/node_time_tmp.get_intend_pkt_num();
			// double pdr = node_time_tmp.get_intend_pkt_num()*1.0/node_time_tmp.get_tx_pkt_num();
			// cout << "avePKTDELAY:" << pktDelay << "ms\t";
			// cout << "PDR:" << pdr << "\t";
			// cout << "intendPkt:" << node_time_tmp.get_intend_pkt_num() << "\t";
			// cout << "txNum:" << node_time_tmp.get_tx_pkt_num() << "\t";
			// cout << endl;
		}

	}
    ofs.close();
}

string TL_SIM_timep::getCurrentTime(){
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

void TL_SIM_timep::print_ave_pkt_delay_consume_all()
{
	vector<int> nodeList;
	get_all_moteid(nodeList);
	print_ave_pkt_delay_nodelist(nodeList);
}



void TL_SIM_timep::get_all_moteid(vector<int>& nodeList)
{
	// vector<int> nodeList;
	for(int i=0 ;i< node_time_list.size() ;i++)
	{
		nodeList.push_back(node_time_list[i].get_moteid());
	}
	// return nodeList;
}





void TL_SIM_timep::update_node_time(TL_SIM_time_node& node_time)
{
	for(int i=0 ;i < node_time_list.size(); i++)
	{
		if(node_time_list[i].get_moteid() == node_time.get_moteid())
		{
			node_time_list[i] = node_time;
			return;
			// node_time = node_time_list[i];
			// return true; 
			// return node_time_list[i];
		}
	}
}








