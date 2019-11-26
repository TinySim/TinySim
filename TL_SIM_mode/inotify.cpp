#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <string.h>
using namespace std;

#define EVENT_SIZE (sizeof(struct inotify_event))
 
void display(struct inotify_event *i,char *path){

	 FILE *fp;
	 char buf[128];
	 memset(buf,'\0',128);

	 printf("wd = %2d;", i->wd);

    if(i -> cookie > 0)
    {
        printf("cookie = %4d;",i->cookie);
    }

    printf("mask = ");

    if(i ->mask & IN_MODIFY) 
       	printf("IN_MODIFY\n");
  
    if((i ->mask & IN_MODIFY)&&(i ->wd == 1))  //判断是那个文件进行了更改
    {
    	printf("file %s has been modified\n", path);
    	fp=fopen(path,"r+");
    	if(fp == NULL){
    		printf("%s","file open error");
    		return;
    	}
    	fgets(buf,128,fp);
    	printf("current content of file is: %s\n", buf);
    	fclose(fp);
    }
    if(i -> len > 0)
    {
        printf("name = %s\n",i->name);
    }
} 
void watch_inotify(char *filepath)
{
    int fd ,wd,ret,length,maxfd;
    char buf[1024] = {0};
    int num = 0;
    fd_set rfds;
    struct timeval tval = {5,0};

    fd = inotify_init();//初始化fd
    if(fd < 0 ){
    	cerr<<"inotify error"<<endl;;
    	return;
    }
    wd = inotify_add_watch(fd, filepath , IN_MODIFY);//监听文件是否修改
    if(wd < 0 ){
    	cerr<<"inotify_add_watch error"<<endl;
    	return ;
    }

   // while(1){
    	FD_ZERO(&rfds);
    	FD_SET(fd,&rfds);
    	maxfd = fd + 1;
    	while(ret =select(maxfd,&rfds,NULL,NULL,&tval) > 0){
    	switch(ret){
    		case -1:
    			cerr<<"select error";
    			break;
    		case 0:
    			//cerr<<"select timeout"<<endl;
    		 	break;
    		 default:
    		 	length = read(fd,buf,1024);
				cout<<"length= "<<length<<endl;
				if(length < 0)
				{
					cerr<<"read error"<<endl;
					return;
				}
				int i = 0;
				while(i < length){
					struct inotify_event *event = (struct inotify_event*)&buf[i];
					printf("event->len = %d\n",event->len);
					display(event,filepath);
					i += EVENT_SIZE + event->len;
				}
				break;
				}
    }
	inotify_rm_watch(fd,wd);
	close(fd);
 }

int main()
{
	char *path = "conn.txt";
    watch_inotify(path);
    return 0;
}

