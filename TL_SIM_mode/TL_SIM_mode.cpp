#include "TL_SIM_mode.h"
#include "CloudDataGen.h"
#include "TL_SIM_powerp.h"
#include "TL_SIM_timep.h"
#include <unistd.h>
#include <thread>


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <string.h>
#define EVENT_SIZE (sizeof(struct inotify_event))



const int TL_SIM_mode::VIRTUAL = 200;
const int TL_SIM_mode::REAL = 100;

const int TL_SIM_mode::LOCK = 1;
const int TL_SIM_mode::GPS  = 2;
const int TL_SIM_mode::COAP  = 3;
const int TL_SIM_mode::NB = 4;
const int TL_SIM_mode::LED = 5;
const int TL_SIM_mode::Temperature = 6;
const int TL_SIM_mode::FAN = 7;
const int TL_SIM_mode::Light = 8;

int TL_SIM_mode::nodenum = 0;
int TL_SIM_mode::startNode = 1;



int TL_SIM_mode::print_state_interval;
vector<int> TL_SIM_mode::stateList;
vector<int> TL_SIM_mode::tempList;

//const int TL_SIM_mode::print_state_interval = 1000; // default 1000 ms

void TL_SIM_mode::setMode(int real_or_virtual){
	// if(real_or_virtual == TL_SIM_mode::VIRTUAL){
	// 	// executing with virtual mode!
	// 	CloudDataGen *clouddata = new CloudDataGen(nodenum,startNode);
	// 	clouddata->setEventNum(10);
	// 	clouddata->executeRequestUnlock(true);
	// }
	thread t(dealCmd);
	t.detach();
}
void TL_SIM_mode::addOutput(int state){
	stateList.push_back(state);
}

void TL_SIM_mode::outputPeriod(int interval){
	print_state_interval = interval;
	thread t(start);
	t.detach();
}

void TL_SIM_mode::start(){
	while(true){
		usleep( print_state_interval * 1000 ); 
		TL_SIM_powerp::print_node_state();
	}
}


void TL_SIM_mode::setNodeConfig(int node_num,int start_Node){
	nodenum = node_num;
	startNode = start_Node;
}


/*
*  Trigger events according to file changes
*  writen at 2018-09-06
*  references to linux inotify mechanism
*/
void analyseCmd(char *cmd){
		string input = cmd;
		if(input == "help"){
			ofstream ofs("../result/other.log",ios::out);
			ofs<< "1. state  		-------print some specified state of node\n" \
				<<"2. power  		-------print power consume of every node\n"  \
				<<"3. delay  		-------print state delay  of every node\n"  \
				<<"4. pkt  			-------print average delay of every node\n"  \
				<<"5. quit       	-------exit the program\n" \
				<<"6. rm            -------remove all hardware states\n" \
				<<"7. rcv        	-------recover all hardware states\n" \
				<<">>";
			ofs.close();
		}
		else if(input == "state"){
			TL_SIM_powerp::print_node_state();
		}
		else if(input == "power"){
			TL_SIM_powerp::print_state_power_consume_all();
		}
		else if(input == "delay"){
			TL_SIM_powerp::print_state_time_consume_all();
		}
		else if(input == "printpktdelay"){
			TL_SIM_timep::print_ave_pkt_delay_consume_all();
		}
		else if(input == "rm"){
			for(int i = 0; i < TL_SIM_mode::stateList.size(); i++){
				TL_SIM_mode::tempList.push_back(TL_SIM_mode::stateList[i]);
			}
			TL_SIM_mode::stateList.clear();
		}
		else if(input == "rcv"){
			for(int i = 0; i < TL_SIM_mode::tempList.size(); i++){
				TL_SIM_mode::stateList.push_back(TL_SIM_mode::tempList[i]);
			}
			TL_SIM_mode::tempList.clear();
			//cout<<">>";
		}
		else if(input == "quit"){
			exit(0);
		}
		else {
			cout<<"please input correct command!" << endl;
		}
}

/*
 * 下面代码是临时添加的,如果需要部署web端的话,需要将这段注释掉
 */
void TL_SIM_mode::dealCmd(){
	string input;
	while(cin>>input){
		if(input == "help"){
			cout<< "1. state  		-------print some specified state of node\n" \
				<<"2. power  		-------print power consume of every node\n"  \
				<<"3. delay  		-------print state delay  of every node\n"  \
				<<"4. pkt  			-------print average delay of every node\n"  \
				<<"5. quit       	-------exit the program\n" \
				<<"6. rm            -------remove all hardware states\n" \
				<<"7. rcv        	-------recover all hardware states\n" \
				<<">>";
			// ofs.close();
		}
		else if(input == "state"){
			TL_SIM_powerp::print_node_state();
		}
		else if(input == "power"){
			TL_SIM_powerp::print_state_power_consume_all();
		}
		else if(input == "delay"){
			TL_SIM_powerp::print_state_time_consume_all();
		}
		else if(input == "printpktdelay"){
			TL_SIM_timep::print_ave_pkt_delay_consume_all();
		}
		else if(input == "rm"){
			for(int i = 0; i < TL_SIM_mode::stateList.size(); i++){
				TL_SIM_mode::tempList.push_back(TL_SIM_mode::stateList[i]);
			}
			TL_SIM_mode::stateList.clear();
		}
		else if(input == "rcv"){
			for(int i = 0; i < TL_SIM_mode::tempList.size(); i++){
				TL_SIM_mode::stateList.push_back(TL_SIM_mode::tempList[i]);
			}
			TL_SIM_mode::tempList.clear();
			//cout<<">>";
		}
		else if(input == "quit"){
			exit(0);
		}
		else {
			cout<<"please input correct command!" << endl;
		}
	}
}

// 以下代码在部署web时使用,主要是监听文件改变去触发一些事件
// void display(struct inotify_event *i,char *path){
// 	FILE *fp;
//     char buf[128];
	
//     if((i ->mask & IN_CLOSE_WRITE)&&(i ->wd == 1))  //判断是那个文件进行了更改
//     {
//     	fp=fopen(path,"r");
//         if(fp == NULL){
//             printf("%s","file open error");
//             return;
//         }
// 	    fgets(buf,128,fp);
// 	    printf("current content of file is: %s\n", buf);
// 	    analyseCmd(buf);
// 	    fclose(fp);
//     }
//     if(i -> len > 0)
//     {
//         printf("name = %s\n",i->name);
//     }
// } 

// void watch_inotify(char *filepath,char *dummy)
// {
//     int fd ,wd,ret,length,maxfd, tempfd;
//     char buf[1024] = {0};
//     int num = 0;
//     fd_set rfds;
//     struct timeval tval = {5,0};

//     fd = inotify_init();
//     if(fd < 0 ){
//     	cerr<<"inotify error"<<endl;;
//     	return;
//     }
//     wd = inotify_add_watch(fd, dummy , IN_CLOSE_WRITE);//监听文件是否修改
//     if(wd < 0 ){
//     	cerr<<"inotify_add_watch error"<<endl;
//     	return ;
//     }

//    while(1){
//     	FD_ZERO(&rfds);
//     	FD_SET(fd,&rfds);
//     	maxfd = fd + 1;
//     	ret =select(maxfd,&rfds,NULL,NULL,&tval);
//     	switch(ret){
//     		case -1:
//     			cerr<<"select error";
//     			break;
//     		case 0:
//     		 	break;
//     		 default:
//                     length = read(fd,buf,1024);
//                         if(length < 0)
//                         {
//                             cerr<<"read error"<<endl;
//                             return;
//                         }
//                         int i = 0;
//                         while(i < length){
//                             struct inotify_event *event = (struct inotify_event*)&buf[i];
//                             display(event,filepath);
//                             i += EVENT_SIZE + event->len;
//                         }
// 				break;
// 				}
//     }
// 	inotify_rm_watch(fd,wd);
// 	close(fd);
//  }


// void TL_SIM_mode::dealCmd(){
// 	char *real = "../TL_SIM_mode/real.sim";
// 	char *dummy = "../TL_SIM_mode/dummy.sim";
// 	watch_inotify(real,dummy);
// }



