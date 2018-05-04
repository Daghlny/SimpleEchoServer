
#include <iostream>
#include <string>
#include <cstdio>

#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("USAGE: server port\n");
        exit(-1);
    }

    int _port = atoi(argv[1]);

    EchoServer server;
    server.bindServer(_port);
    server.start();
    
    return 0;
}
