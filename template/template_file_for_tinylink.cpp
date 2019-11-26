#define MQTTCLIENT_QOS2 1

#include <memory.h>
#include "MQTTClient.h"
#define DEFAULT_STACK_SIZE - 1
#include <iostream>
#include "TL_SIM_schedule.h"

#include <chrono>
#include <thread>
#include <future>

#include "sim_event_util.h"
#include "user_final_file.h"

#include <typeinfo>

using namespace std;
// segment_for_time_module

// segment_for_char_args

void node::setup() {
	// tl_led.setNodeID(get_nodeid());
	// tl_temperature.setNodeID(get_nodeid());

	// socket client配置
	socket_fd = socket(AF_INET, SOCK_STREAM,0);
    if(socket_fd == -1)
    {
        cout<<"create socket failed"<<endl;
        exit(-1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(30001);
    addr.sin_addr.s_addr = inet_addr("10.180.133.38");

    int flags = fcntl(socket_fd, F_GETFL, 0);//获取建立的socket_fd的当前状态（非阻塞）
    fcntl(socket_fd, F_SETFL, flags|O_NONBLOCK);//将当前socket_fd设置为非阻塞

    int res = connect(socket_fd, (struct sockaddr*) &addr, sizeof(addr));
    // if(res == -1)
    // {
    //     cout<<"socket link to server failed"<<endl;
    //     // exit(-1);
    // }

	// segment_for_round_send

	// segment_for_user_setup

	std::shared_future<void> _event_s1 = async(launch::deferred, &node::loop_start, this);
	sim_event  _event_e1(sim_scheduler::currentTime, get_nodeid(), _event_s1);

        ostringstream ost;
		ost << "node::loop_start";
		e1.set_handler_name(ost.str());
	sim_scheduler::sim_queue_insert(_event_e1);
}

void node::loop() {
	// segment_for_user_loop

	loop_end();
}
