#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 6666		/*监听端口*/
#define BACKLOG 5		/*监听队列长度*/
// void process_conn_server(int s);
void *start_routine( void *ptr);
int  main(){
    int ss, sc;			/*ss为服务器套接字描述符
						  sc为客户端的套接字描述符*/
    struct sockaddr_in server_addr;/*服务器地址结构*/
    struct sockaddr_in client_addr;/*客户端地址结果*/
	int err;			/*返回值*/
	// pid_t pid;			/*子进程ID*/
	pthread_t thread; //定义一个线程号
	
	/*建立一个流式套接字*/
	ss = socket(AF_INET, SOCK_STREAM, 0);
    if (ss < 0){		/*出错*/
        printf("Creating socket failed.");
        return -1;
    }
    
    /*设置可重复绑定*/
    int opt = SO_REUSEADDR;
    setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
	/*设置服务器地址*/
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port=htons(PORT);
    
    /*绑定地址结构到套接字描述符*/
    err = bind(ss, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(err < 0){
    	printf("Bind error.");
    	return -1;
    }

    /*设置监听*/
    err = listen(ss,BACKLOG);
    if(err < 0){
    	printf("listen() error\n");
    	return -1;
    }
    
	/*主循环过程*/
    for(;;){
    	int addrlen = sizeof(struct sockaddr);
    	
    	sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
    	/*接收客户端连接*/
    	if(sc < 0){
    		continue;
    	}
    	printf("You got a connection from %s\n",inet_ntoa(client_addr.sin_addr) );
    	/*建立一个新的线程来处理客户端的连接*/
		pthread_create(&thread,NULL,start_routine,(void *)&sc);
    }
	close(ss);
	return(1);
}


void *start_routine(void *ptr){
    int s = *(int *)ptr;
	ssize_t size = 0;
    char buffer[1024];
    
    for(;;){
    	size = read(s, buffer, 1024);
    	
    	if(size == 0){
    		return;
    	}
    	
    	/*格式化处理，将发送的数据数大小发送给客户端*/
    	sprintf(buffer, "%d bytes altogether\n", size);
    	write(s, buffer, strlen(buffer)+1);
    }
	close(s);
}
