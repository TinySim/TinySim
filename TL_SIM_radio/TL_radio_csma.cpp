
#include "TL_radio_csma.h"


int scale = 1;
int TL_radio_csma::txListenDelay = 2*scale; // 10 ms
int TL_radio_csma::maxBackOffSlot = 50;
int TL_radio_csma::slotTime = 2*scale; // 2ms
int TL_radio_csma::maxBackOffTime = TL_radio_csma::maxBackOffSlot * TL_radio_csma::slotTime;



