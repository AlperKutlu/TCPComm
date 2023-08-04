#ifndef SERVER_HPP_
#define SERVER_HPP_


/* #pragma once */

#include <iostream>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <thread>

class SocketComm{
    public:
         
        SocketComm();

        ~SocketComm();

        bool init();

        void ClientComm(int client_socket);
        
        int GetSocket();

        private:

        int m_Socket;
        sockaddr_in m_ServerAddress;
        
};

#endif // SERVER_HPP_