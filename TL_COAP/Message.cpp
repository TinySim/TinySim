#include "Message.h"
Message::Message(){
	this->LOCK = "";
	this->LED = "";
	this->GPS = "";
	this->Temperature = "";
	this->Light = "";
	this->FAN = "";
	this->Moter ="";
	this->NB = "";
}

Message::Message(string LOCK,string LED,string GPS,string Temperature,string Light,string FAN,string Moter,String NB){
	this->LOCK = LOCK;
	this->LED = LED;
	this->GPS = GPS;
	this->Temperature = Temperature;
	this->Light = Light;
	this->FAN = FAN;
	this->Moter = Moter;
	this->NB = NB;
}
void Message::setLockMessage(string lock){
	this->LOCK = lock;
}
void Message::setLedMessage(string led){
	this->LED = led;
}
void Message::setGpsMessgae(string gps){
	this->GPS = gps;
}

void Message::setTemperatureMessage(string temp){
	this->Temperature = temp;
}


void Message::setLightMessage(string light){
	this->Light = light;
}

void Message::setFanMessage(string fan){
	this->FAN = fan;
}


void Message::setMoterMessage(string moter){
	this->Moter = moter;
}
void Message::setNbMessage(string nb){
	this->NB = nb;
}

string Message::getLockMessage(){
	return this->LOCK;
}
string Message::getLedMessage(){
	return this->LED;
}
string Message::getGpsMessage(){
	return this->GPS;
}

string Message::getTemperatureMessage(){
	return this->Temperature;
}


string Message::getLightMessage(){
	return this->Light;
}

string Message::getFanMessage(){
	return this->FAN;
}
string Message::getMoterMessage(){
	return this->Moter;
}
string Message::getNbMessage(){
	return this->NB;
}