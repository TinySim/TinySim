#ifndef TL_SIM_BASESTATION_H_INCLUDED
#define TL_SIM_BASESTATION_H_INCLUDED

#include "TL_NB.h"

class TL_NB_BS
{
public:
	TL_NB_BS(int i_cell_id):cell_id(i_cell_id) {}
	~TL_NB_BS(){}
	

	void MessageArrived(TL_NB::Message& msg);
	void setCellId(int i_cellid);
	int getCellId();

	
private:
	int cell_id;
};




#endif











