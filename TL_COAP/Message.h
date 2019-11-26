#include <iostream>
#include <string>
using namespace std;

// add by qiufujian at 2019-07-17 
// 该实体类定义了服务器返回的消息类型,如果增加了硬件模块,需要增加相应的实体类和get/set方法


class Message{
private:
	// 定义目前现有的各个硬件模块
	string LOCK;
	string LED;
	string GPS;
	string Temperature;
	string Light;
	string FAN;
	string Moter;
	string NB;

public:
	Message();
	Message(string LOCK,string LED,string GPS,string Temperature,string Light,string FAN,string Moter,String NB);
	~Message();
	void setLockMessage(string lock);
	void setLedMessage(string lock);
	void setGpsMessgae(string gps);
	void setTemperatureMessage(string lock);
	void setLightMessage(string lock);
	void setFanMessage(string lock);
	void setMoterMessage(string lock);
	void setNbMessage(string lock);

	string getLockMessage();
	string getLedMessage();
	string getGpsMessage();
	string getTemperatureMessage();
	string getLightMessage();
	string getFanMessage();
	string getMoterMessage();
	string getNbMessage();
}