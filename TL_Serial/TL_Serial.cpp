#include "TL_Serial.h"
#include "TL_SIM_dbg.h"

	
TL_Serial::TL_Serial(){
	this->begin_value = 0;
	TL_SIM_dbg::dbg_add_channel("Serial");
}
void TL_Serial::begin(int value){
	this->begin_value = value;
}
void TL_Serial::print(char val){
	TL_SIM_dbg::dbg("Serial","%c",val);
}
void TL_Serial::print(int val){
	TL_SIM_dbg::dbg("Serial","%d",val);
}
void TL_Serial::print(long val){
	TL_SIM_dbg::dbg("Serial","%ld",val);
}
void TL_Serial::print(double val){
	TL_SIM_dbg::dbg("Serial","%lf",val);
}

void TL_Serial::print(const char* val){
	TL_SIM_dbg::dbg("Serial","%s",val);
}
void TL_Serial::print(const String &val){
	TL_SIM_dbg::dbg("Serial","%s",val.c_str());
}
// -------------------------
void TL_Serial::println(char val){
	TL_SIM_dbg::dbg("Serial","%c\n",val);
}
void TL_Serial::println(int val){
	TL_SIM_dbg::dbg("Serial","%d\n",val);
}
void TL_Serial::println(long val){
	TL_SIM_dbg::dbg("Serial","%ld\n",val);
}
void TL_Serial::println(double val){
	TL_SIM_dbg::dbg("Serial","%lf\n",val);
}
void TL_Serial::println(const char* val){
	TL_SIM_dbg::dbg("Serial","%s\n",val);
}
void TL_Serial::println(const String &val){
	TL_SIM_dbg::dbg("Serial","%s\n",val.c_str());
}