#include <iostream>
#include "U_BLOX_GPS_Arduino_UNO.h"
// #include "UE.h"

using namespace std;

class TL_GPS{
private:
	UBLOX gps;
	int nodeId;
	int xDmen;
	int yDmen;
public:
	// void read(TL_NB_UE ue,TL_NB_UE bs,TL_COAP coap);
	TL_GPS();
	// ~TL_GPS();

	void read();
	double latData();
	double lngData();

	int get_xDmen();
	int get_yDmen();

	void setNodeId(int id);
	int getNodeId();
};
int getRandomTimeConsume();