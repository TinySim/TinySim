#include "TL_radio_link.h"
#include <iostream>
#include <fstream>  
#include <string> 
#include <vector>
#include <math.h>

using namespace std;



double TL_radio_link::getDistance(TL_NB_UE& a, TL_NB_UE& b)
{
	return sqrt( (a.get_xdmen() - b.get_xdmen())*(a.get_xdmen() - b.get_xdmen()) + (a.get_ydmen() - b.get_ydmen())*(a.get_ydmen() - b.get_ydmen()) );
}


// following the friis formulation: https://www.csie.ntu.edu.tw/~hsinmu/courses/_media/wn_11fall/path_loss_and_shadowing.pdf
double TL_radio_link::getSNR(double distance, TL_NB_UE& src, TL_NB_UE& dst)
{
	double txPower = src.get_txPower();
	double noise = dst.get_noise();
	double G_t = 0.0001; // the larger, the snr is better.
	double A_e = 1; // the number of Antenna in receiver;
	double Pt = dBmTowt(txPower);
	double operate_freq = 900 * pow(10, 6);
	double wavelen =  3*pow(10, 8)/ operate_freq;

	// double Pr = (Pt * G_t * A_e * wavelen * wavelen) / (4 * M_PI * distance);
	double Pr = (Pt * G_t * A_e) / (4 * M_PI * distance * distance);
	double dBmPr = wtTodBm(Pr);

	return dBmPr - noise;
}


double TL_radio_link::calcuBPSKErrorRate(double SNR)
{
	return 0.5 * erfc( sqrt(SNR) );

}

double TL_radio_link::calcuQPSKErrorRate(double SNR)
{

}


double TL_radio_link::dBmTowt(double dBm)
{
	return pow(10, dBm/10);
}


double TL_radio_link::wtTodBm(double wt)
{
	return 10*log10(wt);
}

//ref: http://blog.sina.com.cn/s/blog_68f81b8b0100jtse.html
double TL_radio_link::SNRToEbNo(double SNR)
{

}

double TL_radio_link::get_dis()
{
	return this->dis;
}

double TL_radio_link::get_snr()
{
	return this->snr;
}

double TL_radio_link::get_erRate()
{
	return this->erRate;
}


TL_NB_UE TL_radio_link::get_ueSrc()
{
	return this->ueSrc;
}

TL_NB_UE TL_radio_link::get_ueDst()
{
	return this->ueDst;
}