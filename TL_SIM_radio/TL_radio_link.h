#ifndef TL_RADIO_LINK_H_INCLUDED
#define TL_RADIO_LINK_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include "UE.h"

using namespace std;


class TL_radio_link
{
public:
	TL_radio_link(){}
	TL_radio_link(TL_NB_UE& a, TL_NB_UE& b): ueSrc(a), ueDst(b)
	{
		this->dis = getDistance(ueSrc, ueDst);
		this->snr = getSNR(this->dis, ueSrc, ueDst);
		this->erRate = calcuBPSKErrorRate(this->snr);
		
		// cout << a.get_moteid() << " -> " << b.get_moteid() \
		// << ", dis:" << this->dis
		// << ", snr:" << this->snr
		// << ", erRate:" << this->erRate
		// << endl;
	}

	~TL_radio_link(){}


	double getDistance(TL_NB_UE& a, TL_NB_UE& b);
	double getSNR(double distance, TL_NB_UE& src, TL_NB_UE& dst);
	double calcuBPSKErrorRate(double SNR);
	double calcuQPSKErrorRate(double SNR);
	double dBmTowt(double dBm);
	double wtTodBm(double wt);
	double SNRToEbNo(double SNR);



	double get_dis();
	double get_snr();
	double get_erRate();
	TL_NB_UE get_ueSrc();
	TL_NB_UE get_ueDst();



private:
	TL_NB_UE ueSrc;
	TL_NB_UE ueDst;
	double dis;
	double snr;
	double erRate; 

};



#endif