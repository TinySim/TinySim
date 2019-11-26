#include <iostream>
#include "TL_NB.h"
#include "BaseStation.h"

using namespace std;




void TL_NB_BS::MessageArrived(TL_NB::Message& msg)
{
	cout << " [BS]  Receive data " << msg.get_payload() << ", from node " << msg.get_moteid() << endl;
}


void TL_NB_BS::setCellId(int i_cellid)
{
	this->cell_id = i_cellid;
}


int TL_NB_BS::getCellId()
{
	return this->cell_id;
}




