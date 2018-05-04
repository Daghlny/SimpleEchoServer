
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"

#include "Client.hpp"
#include "Tools.hpp"

using std::string;
using std::cin;

int
EchoClient::Connect(string server_ip, int _port)
{
    if ( (sckt = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
        exit_with_errstr("create client socket error");

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(_port);

    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (connect(sckt, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
        exit_with_errstr("connect to server error");

    return 0;
}

int
EchoClient::SendMessage(string msg)
{
    char send_buff[1024];
    char read_buff[1024];
    memset(send_buff, 0, sizeof(send_buff));
    memset(read_buff, 0, sizeof(read_buff));
    strcpy(send_buff, msg.c_str());

    if (send (sckt, send_buff, strlen(send_buff), 0) < 0)
        exit_with_errstr("send error");

    int recv_len;

    if ( (recv_len = recv(sckt, read_buff, 1024, 0)) < 0 )
        exit_with_errstr("recv error");

    printf("GET Respond: %s\n", read_buff);

    return 0;
}

int 
EchoClient::CloseConnect()
{
    close(sckt);
    return 0;
}


