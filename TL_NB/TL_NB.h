#ifndef TL_NB_H_INCLUDED
#define TL_NB_H_INCLUDED

#include <string>
#include "TL_COAP.h"

using namespace std;

class TL_NB
{
public:
	TL_NB(){}
	~TL_NB(){}
   /*
	*  qiufujian
	*/
	void setReceive(int);
	void setSleep(int);
	void init();
	TL_COAP fetchCoap();
	//void setListenTime(int);

	static int NB_IOT_bitrate;


	class Message
	{
		public:
			Message(){}

			Message(string payload, int src, int dst): m_payload(payload), srcid(src), dstid(dst) {}
			
			~Message(){}

			int get_txNum();
			string get_payload();
			int get_srcid();

			void set_txNum(int i_txNum);
			void set_payload(string i_payload);
			void set_srcid(int srcid);
			int  get_dstid();
			void set_dstid(int);


		private:
			string m_payload;
			int srcid;
			int dstid;
			int txNum = 0;
			
	};

private:
	int receivedTime;
	int sleepTime;
	//int listenTime;


};


#endif










