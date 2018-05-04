
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>

#include "unistd.h"
#include "sys/epoll.h"
#include "pthread.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"

using std::queue;

class EchoServer 
{
    public:
        EchoServer();
        int bindServer(int _port);
        int start();
        int listen_to_clients();
        static void *epoll_main_loop(void *);

    private:
        int listen_socket_fd;
        pthread_t listen_thread;

        pthread_mutex_t new_client_q_mutex;

        queue<int> new_client_fd_q;
        int epfd;
};


