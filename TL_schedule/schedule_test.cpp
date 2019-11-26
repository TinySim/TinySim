

#include <iostream>
#include "TL_SIM_schedule.h"
#include <chrono>
#include <thread>
#include <future>
#include <vector>

using namespace std;

void futuF(int a)
{
	cout << "this is " << a <<endl;
}

// struct cmp
// {
// 	bool operator()(sim_event &a, sim_event &b) const
// 	{
// 		return a.time > b.time;
// 	}
// };

int main()
{
	// priority_queue<sim_event, std::vector<sim_event>, cmp> q;

	std::shared_future<void> s1 = async(launch::deferred, futuF, 1);
	std::shared_future<void> s2 = async(launch::deferred, futuF, 2);
	std::shared_future<void> s3 = async(launch::deferred, futuF, 3);






	// sim_event<std::future<void>> e1(100, 1, s1);
	// sim_event<std::future<void>> e2(500, 1, s2);
	// sim_event<std::future<void>> e3(300, 1, s3);


	sim_event e1(100, 1, s1);
	sim_event e2(500, 1, s2);
	sim_event e3(300, 1, s3);


	// sim_event e1(100, 1, std::move(s1));
	// sim_event e2(500, 1, std::move(s2));
	// sim_event e3(300, 1, std::move(s3));


	// sim_event e2 = new sim_event(500, 1);
	// sim_event e3 = new sim_event(300, 1);
	// e1.handler = s1;
	// e2.handler = s2;
	// e3.handler = s3;


	sim_scheduler::sim_queue_insert(e1);
	sim_scheduler::sim_queue_insert(e2);
	sim_scheduler::sim_queue_insert(e3);


	sim_event eo1 = sim_scheduler::sim_queue_pop();
	sim_event eo2 = sim_scheduler::sim_queue_pop();
	sim_event eo3 = sim_scheduler::sim_queue_pop();


	cout << "the event execute at time " << eo1.get_time() << ", at node " << eo1.get_mote() << endl;
	eo1.get_handler().wait();

	cout << "the event execute at time " << eo2.get_time() << ", at node " << eo2.get_mote() << endl;
	eo2.get_handler().wait();

	cout << "the event execute at time " << eo3.get_time() << ", at node " << eo3.get_mote() << endl;
	eo3.get_handler().wait();

	return 0;
}

