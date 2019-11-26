#include <iostream>

using namespace std;

class TL_SIM_Socket{
private:
	int socket_fd;
	string socket_msg;
	char socket_send[100];
	char socket_receive[100];
public:
	TL_SIM_Socket();
	void sendMsg(string msg);
	void receiveMsg();
};
