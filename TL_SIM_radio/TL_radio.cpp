#include "TL_radio.h"
#include "TL_radio_trace.h"
#include "TL_radio_link.h"
#include "UE.h"

#include <iostream>
#include <fstream>  
#include <string> 
#include <vector>
#include <math.h>


using namespace std;

vector<TL_radio_link> TL_radio::linkList;

void TL_radio::genTopoFromTrace(string topoFile)
{

	TL_radio_trace::readTopoTrace(topoFile);

	// currnetly, it only needs to construct the link between the BS and the UE.
	// but now,there may be many BSs. To simplify this situation, we make sure that one node is just covered by only one BS at the same time. refreshed by qiufujian at 2018-8-22
	for(int i=0 ; i< TL_radio_trace::ueList.size() ; i++)
	{
		TL_NB_UE ue,bs;
		ue = TL_radio_trace::ueList[i];
		if(!TL_radio_trace::ueTobs.count(ue)){
			continue;
		}
		bs = TL_radio_trace::ueTobs[ue];
		TL_radio_link rlink(ue,bs);
		TL_radio::linkList.push_back(rlink);
		TL_radio_link rlink2(bs, ue);
		TL_radio::linkList.push_back(rlink2);
	}

	// for(int i=0 ; i< TL_radio_trace::ueList.size() ; i++)
	// {
	// 	for(int j=0 ; j< TL_radio_trace::ueList.size(); j++)
	// 	{
	// 		if( i == j)
	// 		{
	// 			continue;
	// 		}

	// 		TL_radio_link rlink(TL_radio_trace::ueList[i], TL_radio_trace::ueList[j]);
	// 		TL_radio::linkList.push_back(rlink);
	// 	}
	// }


}


void TL_radio::genNoiseFromTrace(string noiseFile)
{

}



TL_radio_link* TL_radio::findLink(int ueIdSrc, int ueIdDst)
{
	for(int i=0 ;i< TL_radio::linkList.size() ;i++)
	{
		if(TL_radio::linkList[i].get_ueSrc().get_moteid() == ueIdSrc && \
			TL_radio::linkList[i].get_ueDst().get_moteid() == ueIdDst)
		{
			return &TL_radio::linkList[i];
		}
	}
	// return *(TL_radio_link*)NULL;
	return NULL;
}

void TL_radio::getNeighborsFromRxId(int rxId, vector<int> &neighborList )
{

	for(int i = 0; i < TL_radio::linkList.size(); i++)
	{
		if(TL_radio::linkList[i].get_ueDst().get_moteid() == rxId)
		{
			int tempNeighborId = TL_radio::linkList[i].get_ueSrc().get_moteid();
			neighborList.push_back(tempNeighborId);
		}
		// else if(TL_radio::linkList[i].get_ueSrc().get_moteid() == rxId)
		// {
		// 	int tempNeighborId = TL_radio::linkList[i].get_ueDst().get_moteid();
		// 	neighborList.push_back(tempNeighborId);
		// }

	}
	

}



