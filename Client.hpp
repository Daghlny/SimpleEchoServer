
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdio>

#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"

using std::string;

class EchoClient {
    public:
        EchoClient() {}
        int Connect(string server_ip, int _port);
        int SendMessage(string msg);
        int CloseConnect();

    private:
        int sckt;
};

