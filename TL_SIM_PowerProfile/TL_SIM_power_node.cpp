#include "TL_SIM_power_node.h"


void TL_SIM_power_node::set_moteid(int i_moteid)
{
	this->moteid = i_moteid;
}

// the power is just calcualted from the consumed time, and thus only set the consumed power.
void TL_SIM_power_node::set_state_power_consume(int state, double powercons)
{
	this->state_power_consume[state] = powercons;
}


// void TL_SIM_power_node::set_pre_state(int state)
// {
// 	this->pre_state = state;
// }
// void TL_SIM_power_node::set_pre_time(int time)
// {
// 	this->pre_time = time; 
// }

void TL_SIM_power_node::set_NB_pre_state(int state)
{
	this->NB_pre_state = state;
}
void TL_SIM_power_node::set_NB_pre_time(int time)
{
	this->NB_pre_time = time; 
}

void TL_SIM_power_node::set_GPS_pre_state(int state)
{
	this->GPS_pre_state = state;
}
void TL_SIM_power_node::set_GPS_pre_time(int time)
{
	this->GPS_pre_time = time; 
}

void TL_SIM_power_node::set_LOCK_pre_state(int state)
{
	this->LOCK_pre_state = state;
}
void TL_SIM_power_node::set_LOCK_pre_time(int time)
{
	this->LOCK_pre_time = time; 
}

void TL_SIM_power_node::set_COAP_pre_state(int state)
{
	this->COAP_pre_state = state;
}
void TL_SIM_power_node::set_COAP_pre_time(int time)
{
	this->COAP_pre_time = time; 
}


void TL_SIM_power_node::set_LED_pre_state(int state)
{
	this->LED_pre_state = state;
}
void TL_SIM_power_node::set_LED_pre_time(int time)
{
	this->LED_pre_time = time; 
}



// add by qiufj at 2019-07-16 19:06
void TL_SIM_power_node::set_Temperature_pre_state(int state)
{
	this->Temperature_pre_state = state;
}
void TL_SIM_power_node::set_Temperature_pre_time(int time)
{
	this->Temperature_pre_time = time; 
}

void TL_SIM_power_node::set_FAN_pre_state(int state)
{
	this->FAN_pre_state = state;
}
void TL_SIM_power_node::set_FAN_pre_time(int time)
{
	this->FAN_pre_time = time; 
}
void TL_SIM_power_node::set_Light_pre_state(int state)
{
	this->Light_pre_state = state;
}
void TL_SIM_power_node::set_Light_pre_time(int time)
{
	this->Light_pre_time = time; 
}



void TL_SIM_power_node::update_state_time_consume(int state, int time)
{
	this->state_time_consume[state] += time;
}


int TL_SIM_power_node::get_moteid()
{
	return this->moteid; 
}
double TL_SIM_power_node::get_state_power_consume(int index)
{
	return this->state_power_consume[index];
}
// int TL_SIM_power_node::get_pre_state()
// {
// 	return this->pre_state;
// }
// int TL_SIM_power_node::get_pre_time()
// {
// 	return this->pre_time;
// }

int TL_SIM_power_node::get_NB_pre_state()
{
	return this->NB_pre_state;
}
int TL_SIM_power_node::get_NB_pre_time()
{
	return this->NB_pre_time;
}

int TL_SIM_power_node::get_GPS_pre_state()
{
	return this->GPS_pre_state;
}
int TL_SIM_power_node::get_GPS_pre_time()
{
	return this->GPS_pre_time;
}

int TL_SIM_power_node::get_LOCK_pre_state()
{
	return this->LOCK_pre_state;
}
int TL_SIM_power_node::get_LOCK_pre_time()
{
	return this->LOCK_pre_time;
}

int TL_SIM_power_node::get_COAP_pre_state()
{
	return this->COAP_pre_state;
}
int TL_SIM_power_node::get_COAP_pre_time()
{
	return this->COAP_pre_time;
}
int TL_SIM_power_node::get_LED_pre_state()
{
	return this->LED_pre_state;
}
int TL_SIM_power_node::get_LED_pre_time()
{
	return this->LED_pre_time;
}


// add by qiufj at 2019-07-16 19:07
int TL_SIM_power_node::get_Temperature_pre_state()
{
	return this->Temperature_pre_state;
}
int TL_SIM_power_node::get_Temperature_pre_time()
{
	return this->Temperature_pre_time;
}

int TL_SIM_power_node::get_FAN_pre_state()
{
	return this->FAN_pre_state;
}
int TL_SIM_power_node::get_FAN_pre_time()
{
	return this->FAN_pre_time;
}

int TL_SIM_power_node::get_Light_pre_state()
{
	return this->Light_pre_state;
}
int TL_SIM_power_node::get_Light_pre_time()
{
	return this->Light_pre_time;
}
// 




int TL_SIM_power_node::get_LAST_LOCK_state(){
	return this->Last_LOCK_state;
}

void TL_SIM_power_node::set_LAST_LOCK_state(int state){
	this->Last_LOCK_state = state;
}


int TL_SIM_power_node::get_state_time_consume(int index)
{
	return this->state_time_consume[index];
	// i_state_time_consume = this->state_time_consume;
}

