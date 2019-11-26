#ifndef UE_H_INCLUDED
#define UE_H_INCLUDED

#include "TL_NB.h"
#include "TL_SIM_power_util.h"
#include "MQTTClient.h"
#include "TL_SIM_time_util.h"
#include "TL_SIM_schedule.h"

// class TL_SIM_powerp;


// 定义一个全局的存储结构,产生一个节点就加入其中



class TL_NB_UE
{
  public:
	TL_NB_UE() {}
	TL_NB_UE(int moteid)
	{
		this->m_moteid = moteid;
	}
	bool operator < (const TL_NB_UE & dummy) const{
		return m_moteid < dummy.get_moteid();
	}
	~TL_NB_UE() {}

	void retxMessage(TL_NB::Message msg, TL_NB_UE bs);
	void sendMessage(TL_NB::Message msg, TL_NB_UE bs);
	void receiveMessage(TL_NB::Message msg);
	// void retxMessage(TL_NB::Message& msg, TL_NB_UE& bs);
	// void sendMessage(TL_NB::Message& msg, TL_NB_UE& bs);
	// void receiveMessage(TL_NB::Message& msg);
	void join(int cellid);
	void goToSleep();
	void goToListen();

	int get_maxTxTime();
	int get_currentTxTime();
	int get_moteid() const; // modified by qiufujian at 2018-8-23
	int get_txPower();
	int get_noise();
	int get_xdmen();
	int get_ydmen();
	int get_isSink();
	int get_state();
	int get_SleepTime();

	void set_moteid(int);
	void set_xdmen(int);
	void set_ydmen(int);
	void set_txPower(int);
	void set_noise(int);
	void set_isSink(bool);
	void set_state(int);
	void set_SleepTime(int);

	int get_bootTime();
	void set_bootTime(int);

	double get_coverRange();
	void set_coverRange(double);

	int get_carryCapacity();
	void set_carryCapacity(int);

	int get_currentCarry();
	void set_currentCarry(int);

	void set_nextWakeupTime(int time);
	int  get_nextWakeupTime();

	// this method is leaved for developer to rewrite.
	// void messageArrived(MQTT::MessageData& md); // this is the real arrive msg.
	void messageArrived(MQTT::Message &md);

 private:
	int maxTxTime = 20;
	int currentTxTime = 0;
	int sleepTime;
	int m_moteid;
	int xdmen;
	int ydmen;
	int txPower = 20;
	int noise = -100;
	bool isSink = false;
	int bootTime = -1;
	int ListenTime;

	int coverRange = 0; // if this ue is a base, we need sto specify the coverage of the base station.
	int carryCapacity = 0; // the quantity of nodes that one gateway can communicate with at one moment.
	int currentCarry = 0;

	int state = TL_SIM_power_util::S_SHOTDOWN;
	// int state = TL_SIM_power_util::S_IDLE;

	int nextWakeTime;
};

#endif
