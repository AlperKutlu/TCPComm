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
#include <string.h>
#include <yaml-cpp/yaml.h>

class SocketComm{
    public:
         
        SocketComm();

        ~SocketComm();

        bool init();

        void ClientComm(int client_socket);
        
        int GetSocket();

        void SetPort(int PortNum);

        private:

        int m_Socket;
        sockaddr_in m_ServerAddress;
        int m_PortNum;
        int m_IPAddr;
        
};

#endif // SERVER_HPP_