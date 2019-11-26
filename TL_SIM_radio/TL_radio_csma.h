#ifndef TL_RADIO_CAMA_H_INCLUDED
#define TL_RADIO_CAMA_H_INCLUDED

class TL_radio_csma
{
public:
	TL_radio_csma(){}
	~TL_radio_csma(){}		

	static int txListenDelay; 
	static int maxBackOffSlot;
	static int slotTime;
	static int maxBackOffTime;

};



#endif










