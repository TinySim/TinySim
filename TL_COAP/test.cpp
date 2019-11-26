#include <iostream>
extern "C"{
	#include "client.h"
}
// #include "client.h"
using namespace std;

int main(){
	char* url = "coap://localhost:5683/hello";
	char ip[10] = "localhost";
	char port[5] = "5683";

	char* result = coap_send_data_by_get(url,ip,port);
	cout << result<< endl;
}

