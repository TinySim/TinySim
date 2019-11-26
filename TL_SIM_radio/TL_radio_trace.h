#ifndef TL_RADIO_TRACE_H_INCLUDED
#define TL_RADIO_TRACE_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include "TL_radio_link.h"
#include "UE.h"


using namespace std;


class TL_radio_trace
{
public:
	TL_radio_trace(){}
	~TL_radio_trace(){}

	static vector<TL_NB_UE> ueList;
	static vector<TL_NB_UE> bsList;
	//static TL_NB_UE bs;
	static map<TL_NB_UE,TL_NB_UE> ueTobs;

	static void readTopoTraceSNR(vector<string>& topoFile);
	static void readTopoTracePosition(vector<string>& topoFile);
	static void readTopoTrace(string topoFile);
	static vector<string> SplitString(const string& s, const string& c);
	static void readNoiseTrace(string noiseFile);
	// static int getUENum();
	static int getUEListIndex(int moteid);
	static void printUEListAndBs();

	static TL_NB_UE* findUEById(int id); 

	static void createNodeGatewayMap(vector<TL_NB_UE> &UeNodeList,vector<TL_NB_UE>& BaseStationList);


};



#endif