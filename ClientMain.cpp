
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>

#include "Client.hpp"

using std::string;
using std::cin;

int main(int argc, char **argv)
{

    if (argc < 3) {
        printf("USAGE: client server_ip server_port\n");
        exit(-1);
    }

    EchoClient client;

    client.Connect(argv[1], atoi(argv[2]));
    printf("Connected to server(%s:%d)\n", argv[1], atoi(argv[2]));
    while (1) {
        string msg;
        cin >> msg;
        if (msg == "") {
            client.CloseConnect();
            break;
        }
        client.SendMessage(msg);
    }
    
    return 0;
}
