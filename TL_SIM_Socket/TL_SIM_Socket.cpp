#include "TL_SIM_Socket.h"

TL_SIM_Socket::TL_SIM_Socket() {
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
    addr.sin_addr.s_addr = inet_addr("192.168.1.222");

    int flags = fcntl(socket_fd, F_GETFL, 0);//获取建立的socket_fd的当前状态（非阻塞）
    fcntl(socket_fd, F_SETFL, flags|O_NONBLOCK);//将当前socket_fd设置为非阻塞

    int res = connect(socket_fd, (struct sockaddr*) &addr, sizeof(addr));
    // if(res == -1)
    // {
    //     cout<<"socket link to server failed"<<endl;
    //     // exit(-1);
    // }
}

void sendMsg(string msg) {
	socket_msg = msg;
    memset(socket_send, '\0', sizeof(char) * 100);
    for(int i=0; i<socket_msg.size(); i++)
    {
        socket_send[i] = socket_msg[i];
    }
    write(socket_fd, socket_send, 30);
    cout << "[send]" << socket_send << endl;
}

void receiveMsg() {
	;
}
