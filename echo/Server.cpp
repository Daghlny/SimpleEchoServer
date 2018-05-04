
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>

#include "unistd.h"
#include "pthread.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/epoll.h"
#include "netinet/in.h"

#include "Server.hpp"
#include "Tools.hpp"

using std::string;
using std::queue;
using std::cout;
using std::endl;
using std::to_string;

EchoServer::EchoServer()
{

}

/*
 * create a listen socket and bind Server Class to a port
 */
int 
EchoServer::bindServer(int _port)
{
    if ( (listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        exit_with_errstr("open listen socket error");

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(_port);

    if ( (bind(listen_socket_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) == -1 )
        exit_with_errstr("bind to port error");
    

}

/*
 * runs in another thread different with the listening thread
 * when accepted socket queue is not empty, move it from queue to epfd instance
 * when a client socket has data to read, read it and send it back with a time stamp
 */
void *
EchoServer::epoll_main_loop(void* arg)
{
    EchoServer *M = (EchoServer*)arg;

    cout << "Begin Recv Clients ..." << endl;
    struct epoll_event *_clients = (struct epoll_event*) malloc(sizeof(struct epoll_event) * 10);
    char recv_buff[1024];
    char send_buff[1024];
    while (1)
    {
        pthread_mutex_lock(& (M->new_client_q_mutex));
        while (!M->new_client_fd_q.empty()) {
            printf("new client in queue\n");
            struct epoll_event new_client_event;
            new_client_event.events = 0x00 | EPOLLIN;
            new_client_event.data.fd = M->new_client_fd_q.front();
            epoll_ctl(M->epfd, EPOLL_CTL_ADD, M->new_client_fd_q.front(), &new_client_event);
            M->new_client_fd_q.pop();
        }
        pthread_mutex_unlock(& (M->new_client_q_mutex));

        int nclients = epoll_wait(M->epfd, _clients, 2, 1000);
        if (nclients == -1) {
            printf("epoll_wait has a error: %s(errno: %d)\n", strerror(errno), errno);
            exit(-1);
        }
        //printf("epoll_wait return with %d client ready\n", nclients);

        for (int i = 0; i < nclients; ++i) 
        {
            cout << nclients << endl;
            if (_clients[i].events & EPOLLIN) {
                memset(recv_buff, 0, sizeof(recv_buff));
                int recv_len = recv(_clients[i].data.fd, recv_buff, 1024, 0);
                if (recv_len < 0) 
                    exit_with_errstr("recv error");
                time_t now_time = time(NULL);
                tm *real_time = localtime(&now_time);
                string time_str = to_string(real_time->tm_mon) + "/" + to_string(real_time->tm_mday);
                time_str += "[" + to_string(real_time->tm_hour) + ":" + to_string(real_time->tm_min);
                time_str += ":" + to_string(real_time->tm_sec) + "] ";
                time_str += recv_buff;
                memset(send_buff, 0, sizeof(send_buff));
                memcpy(send_buff, time_str.c_str(), time_str.size());

                if (send(_clients[i].data.fd, send_buff, sizeof(send_buff), 0) == -1)
                    exit_with_errstr("send error");
            }
        }
    }

    return NULL;
}

/*
 * starts the server to listen to the _port
 * everytime accepts a connection from socket, add it into queue
 */
int
EchoServer::start()
{
    if ( (listen(listen_socket_fd, 10)) == -1 ) 
        exit_with_errstr("listen error");

    epfd = epoll_create(10);
    if (epfd == -1)
        cout << "epoll_create() error" << " " << strerror(errno) << endl;

    pthread_mutex_init( &new_client_q_mutex, NULL );

    if ( (pthread_create(&listen_thread, NULL, epoll_main_loop, (void*)this)) != 0 )
        exit_with_errstr("thread create error");

        
    while (1) {
        int client_fd = 0;
        if ( (client_fd = accept(listen_socket_fd, (struct sockaddr*)NULL, NULL)) == -1 ) {
            printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
            continue;
        }
        printf("Receive a new Client\n");
        pthread_mutex_lock(&new_client_q_mutex);
        new_client_fd_q.push(client_fd);
        pthread_mutex_unlock(&new_client_q_mutex);
    }

}


