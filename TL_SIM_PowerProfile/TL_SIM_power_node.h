#ifndef TL_SIM_POWER_NODE_H_INCLUDED
#define TL_SIM_POWER_NODE_H_INCLUDED 

#include "TL_SIM_power_util.h"
#include <vector>

class TL_SIM_power_node
{
public:
	TL_SIM_power_node()
	{
		for (int i = 0; i < TL_SIM_power_util::total_state_num; ++i)
		{
			state_power_consume.push_back(0);
			state_time_consume.push_back(0);
		}

	}
	~TL_SIM_power_node(){}

	void set_moteid(int);
	void set_state_power_consume(int, double);
	void update_state_time_consume(int, int);

	// void set_pre_state(int);
	// void set_pre_time(int);

	void set_NB_pre_state(int);
	void set_NB_pre_time(int);

	void set_GPS_pre_state(int);
	void set_GPS_pre_time(int);

	void set_LOCK_pre_state(int);
	void set_LOCK_pre_time(int);

	 void set_COAP_pre_state(int);
	 void set_COAP_pre_time(int);

	 void set_LED_pre_state(int);
	 void set_LED_pre_time(int);



	 // below add by qiufujian at 2019-07-16
	 void set_Temperature_pre_state(int);
	 void set_Temperature_pre_time(int);


	 void set_FAN_pre_state(int);
	 void set_FAN_pre_time(int);

	 void set_Light_pre_state(int);
	 void set_Light_pre_time(int);





	int get_moteid();
	double get_state_power_consume(int);
	int get_state_time_consume(int);

	// int get_pre_state();
	// int get_pre_time();

	int get_NB_pre_state();
	int get_NB_pre_time();

	int get_GPS_pre_state();
	int get_GPS_pre_time();

	int get_LOCK_pre_state();
	int get_LOCK_pre_time();


	int get_COAP_pre_state();
	int get_COAP_pre_time();

	int get_LED_pre_state();
	int get_LED_pre_time();


	// add by qiufujian at 2019-07-16 19:04 
	int get_Temperature_pre_state();
	int get_Temperature_pre_time();

	int get_FAN_pre_state();
	int get_FAN_pre_time();

	int get_Light_pre_state();
	int get_Light_pre_time();




	int get_LAST_LOCK_state();
	void set_LAST_LOCK_state(int state);

	int update_four_module_state();
	int get_four_module_state(int index);


private:
	int moteid;
	vector<double> state_power_consume;
	vector<int>  state_time_consume;

	int Last_LOCK_state = 6; 


	// double state_power_consume[TL_SIM_power_util::S_STATE_NUM] = {0};
	// int state_time_consume[TL_SIM_power_util::S_STATE_NUM] = {0};
	// int pre_state;
	// int pre_time;

	int NB_pre_time;
	int NB_pre_state;

	int GPS_pre_time;
	int GPS_pre_state;

	int LOCK_pre_time;
	int LOCK_pre_state;

	int COAP_pre_time;
	int COAP_pre_state;

	int LED_pre_time;
	int LED_pre_state;


	// below add by qiufujian at 2019-07-16

	int Temperature_pre_time;
	int Temperature_pre_state;

	int FAN_pre_time;
	int FAN_pre_state;

	int Light_pre_time;
	int Light_pre_state;

	//int four_moudle_state[4];

	
};



#endif












