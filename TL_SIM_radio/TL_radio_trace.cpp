#include "TL_radio_trace.h"
#include <iostream>
#include <fstream>  
#include <string> 
#include <vector>
#include <climits>

using namespace std;



vector<TL_NB_UE> TL_radio_trace::ueList;
vector<TL_NB_UE> TL_radio_trace::bsList;
map<TL_NB_UE,TL_NB_UE> TL_radio_trace::ueTobs;


//TL_NB_UE TL_radio_trace::bs;



void TL_radio_trace::readNoiseTrace(string noiseFile)
{

}


// static int TL_radio_trace::getUENum()
// {
// 	return TL_radio_trace::ueList.size();
// }

void TL_radio_trace::readTopoTraceSNR(vector<string>& topoFile)
{
	// [Warning] currently, not support for the SNR trace reading and analysis.
	cout << "[Warning] currently, not support for the SNR trace reading and analysis." << endl;
	for(int i=0 ;i< topoFile.size() ;i++)
	{
		vector<string> v = SplitString(topoFile[i], " ");
		for(int i = 0; i != v.size(); ++i)
			cout << v[i] << ", ";
		cout << endl;
	}

}

/*
* when  building the relationship between gateways and nodes,we need to know the cover range and carrying capacity of the gateway
* author: qiufujian
* date : 2018-8-22
*/
void TL_radio_trace::createNodeGatewayMap(vector<TL_NB_UE> &UeNodeList,vector<TL_NB_UE>& BaseStationList){
	cout << "节点与基站的关联关系：" << endl;
	for (int i = 0; i<UeNodeList.size(); i++){
		int index = 0;
		int min_distance = INT_MAX;
		for (int j=0; j< BaseStationList.size(); j++){
			int x_differ = BaseStationList[j].get_xdmen() - UeNodeList[i].get_xdmen();
			int y_differ = BaseStationList[j].get_ydmen() - UeNodeList[i].get_ydmen();
			double range = BaseStationList[j].get_coverRange();
			int distance = x_differ * x_differ + y_differ * y_differ;


			// 一个节点需要与一个基站建立关联需要满足以下三个条件
			// 1. 已经建立联系的节点的数量小于当前基站的最大负载
			// 2. 该节点与基站的距离不超过基站的覆盖范围
			// 3. 该节点与当前基站之间的距离是到所有基站之间最小的

			if(BaseStationList[j].get_currentCarry() < BaseStationList[j].get_carryCapacity() &&
				double(distance) <= range * range ){
					if(min_distance >= distance){
						min_distance = distance;
						index = j;
					}	
			}
		}
		ueTobs.insert(pair<TL_NB_UE,TL_NB_UE>(UeNodeList[i],BaseStationList[index]));
		BaseStationList[index].set_currentCarry(BaseStationList[index].get_currentCarry() + 1);
		cout << UeNodeList[i].get_moteid() << "   "<<BaseStationList[index].get_moteid() <<endl;
	}
}

void TL_radio_trace::readTopoTracePosition(vector<string>& topoFile)
{
	int init_var = 0;
	bool flag = true;
	for(int i=0 ;i< topoFile.size() ;i++)
	{
		vector<string> v = SplitString(topoFile[i], " ");
		int moteid = stoi(v[0]);
		int xdmen = stoi(v[1]);
		int ydmen = stoi(v[2]);

		TL_NB_UE ue(moteid);

		ue.set_xdmen(xdmen);
		ue.set_ydmen(ydmen);

		if (init_var > moteid){
			flag = false;
		}
		else{
			init_var = moteid;
		}
		
		if (flag == true ) // in default, the first node is the sink.(but now,there may be many gateways.refresh at 2018-8-22)
		{
			// 理论来讲，每个基站的覆盖范围大约2~3km，节点承载量大约50 000个，考虑到实际损耗
			// 这里采用每个节点一万个的承载量

			ue.set_isSink(true);
			ue.set_coverRange(2500); // given that each base can cover one hundred nodes.
			ue.set_carryCapacity(10000);
			ue.set_currentCarry(0);
			TL_radio_trace::bsList.push_back(ue);
		}
		else
		{
			TL_radio_trace::ueList.push_back(ue);
		}

	}
	createNodeGatewayMap(ueList, bsList);
}

void TL_radio_trace::readTopoTrace(string topoFile)
{
	string fileName = topoFile;  
	string s;
	ifstream  inFile;  
	inFile.open(fileName.c_str());  
	if (!inFile){  
		cerr<<"Open file error!"<<endl;  
		return;  
	}  
	getline(inFile,s);
	vector<string> v_type = SplitString(s, " ");
	vector<string> v_topfile;
	while (getline(inFile, s)){
		v_topfile.push_back(s);
	}
	if (v_type[0] == "distance")
	{
		readTopoTracePosition(v_topfile);
	}
	else if(v_type[0] == "snr")
	{
		readTopoTraceSNR(v_topfile);
	}

	inFile.close();
}



vector<string> TL_radio_trace::SplitString(const string& s, const string& c)
{
	std::string::size_type pos1, pos2;
	std::vector<std::string> v;
	pos2 = s.find(c);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if(pos1 != s.length())
		v.push_back(s.substr(pos1));
	return v;
}


int TL_radio_trace::getUEListIndex(int moteid)
{
	for(int i=0 ; i<ueList.size() ; i++)
	{
		if( ueList[i].get_moteid() == moteid)
		{
			return i;
		}
	}
}


void TL_radio_trace::printUEListAndBs()
{
	cout << "==== [Show all ue and bs info] ====\n";
	for(int i=0 ; i< ueList.size() ;i++)
	{
		cout << "[Node " << ueList[i].get_moteid() << "], bootTime:" << ueList[i].get_bootTime() \
			<< ", SleepTime:" << ueList[i].get_SleepTime() \
			<< ", get_nextWakeupTime:" << ueList[i].get_nextWakeupTime() \
			// << ", get_listenTime:" << ueList[i].get_listenTime() 
			<< ", [SINK]:" << ueList[i].get_isSink()
			<< endl;
	}
	for(int i=0 ; i< bsList.size() ;i++)
	{
		cout << "[BaseStation " << bsList[i].get_moteid() << "], bootTime:" << bsList[i].get_bootTime() \
			<< ", SleepTime:" << bsList[i].get_SleepTime() \
			<< ", get_nextWakeupTime:" << bsList[i].get_nextWakeupTime() \
			<< ", [SINK]:" << bsList[i].get_isSink()
			<< endl;
	}

}

// 根据节点ID返回节点对象
// add by qiufj at 2019-07-17

TL_NB_UE* TL_radio_trace::findUEById(int id){
	for(int i = 0;i< ueList.size() + bsList.size();i++){
		if(i < ueList.size() && id == ueList[i].get_moteid()){
			return &ueList[i];
		}
		else if(id == bsList[i - ueList.size()].get_moteid()){
			return &bsList[i - ueList.size()];
		}
	}
	return NULL;
}

