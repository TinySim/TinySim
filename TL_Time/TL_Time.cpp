#include "TL_Time.h"

void TL_Time::delayMillis(int millsec){
	usleep(millsec * 1000);
	;
}
