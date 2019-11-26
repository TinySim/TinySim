#include "TL_NB.h"
#include <string>

using namespace std;

// TL_NB::Message(string payload, int src, int dst): m_payload(payload), srcid(src), dstid(dst) {}
// // if there is only src mote id, then we default set the dst id as the bs id.
// // TL_NB::Message(string payload, int src): m_payload(payload), srcid(src) 
// // {
// // 	this->dstid = TL_radio_trace::bs.get_moteid();
// // }
// ~TL_NB::Message(){}


int TL_NB::NB_IOT_bitrate = 2000000;



int TL_NB::Message::get_txNum()
{
	return this->txNum;
}

std::string TL_NB::Message::get_payload()
{
	return this->m_payload;
}

int TL_NB::Message::get_srcid()
{
	return this->srcid;
}


void TL_NB::Message::set_txNum(int i_txNum)
{
	this->txNum = i_txNum;
}

void TL_NB::Message::set_payload(string i_payload)
{
	this->m_payload = i_payload;
}

void TL_NB::Message::set_srcid(int srcid)
{
	this->srcid = srcid;
}

int TL_NB::Message::get_dstid()
{
	return this->dstid;
}
void TL_NB::Message::set_dstid(int dst)
{
	this->dstid = dst;
}



/*
* qiufujian 2018-04-09
*/
void TL_NB::setSleep(int sleeptime){
	this->sleepTime = sleeptime;

}


void TL_NB::setReceive(int receivedtime){
	this->receivedTime = receivedtime;
}


void TL_NB::init(){
	this->sleepTime = 1000;
	this->receivedTime = 100;
}

TL_COAP TL_NB::fetchCoap(){
	TL_COAP coap;
	coap.setMacType("NB");
	return coap;
}

