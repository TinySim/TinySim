#ifndef TL_RADIO_H_INCLUDED
#define TL_RADIO_H_INCLUDED

#include <vector>
#include <string>
#include "TL_radio_link.h"

using namespace std;


class TL_radio
{
public:
	TL_radio(){}
	~TL_radio(){}


	static vector<TL_radio_link> linkList;
	static void genTopoFromTrace(string topoFile);
	static void genNoiseFromTrace(string noiseFile);
	static TL_radio_link* findLink(int ueIdSrc, int ueIdDst);
	static void getNeighborsFromRxId(int rxId, vector<int> &neighborList );

};


#endif
