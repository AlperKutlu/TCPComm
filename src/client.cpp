#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <fstream>

const  int max_iteration = 100;


void receive_messages(int socket)
{
    char buffer[1024] = {0};

    int iteration = 0;

    while (iteration < max_iteration)
    {
        //receive message from server
        int recv_len = recv(socket, buffer, 1024, 0);

        if(recv_len == -1) 
        {
            std::cerr << "Failed to receive message from server." << std::endl;
            memset(buffer, 0, sizeof(buffer));
            
            break;
        }
        std::cout << "Server message: " << buffer << std::endl;

        //clear buffer
        memset(buffer, 0, sizeof(buffer));

        iteration++;
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 4){
        std::cerr << "Usage: " << argv[0] << "<server_ip> <port_number>" << std::endl;

        return 1;
    }

    std::string server_ip = argv[1];
    int port_number = std::stoi(argv[2]);
    std::string json_file_path = argv[3];

    

    //create a socket

    int socket2 = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket2 == -1)
    {
        std::cerr << "Failed to create socket." << std::endl;

        return 1;        
    }

    //connect to server

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip.c_str());

    //connect to local host

    server_address.sin_port = htons(port_number);

    if(connect(socket2, (struct sockaddr *)&server_address, sizeof(server_address)) ==1)
    {
        std::cerr <<"Failed to connect to server." << std::endl;

        close(socket2);

        return 1;
    }

    //read JSON content from the file
    
    std::ifstream json_file(json_file_path);
    if (!json_file) 
    {
        std::cerr << "Failed to open JSON file." << std::endl;
        
        return 1;
    }
    std::string json_content((std::istreambuf_iterator<char>(json_file)), std::istreambuf_iterator<char>());
    std::cout << "Size:" << json_content.size() << std::endl;
    std::cout << json_content << std::endl;

    //send JSON message to server

     int send_len = send(socket2, json_content.c_str(), json_content.length(), 0);

    if(send_len == -1) 
    {
        std::cerr << "Failed to send JSON message to server." << std::endl;
        close(socket2);
        return 1;

    }

    
    /*send message to server
    std::string message;
    
    for(int i = 0; i<max_iteration; i++)
    {
        std::getline(std::cin, message);
        int send_len = send(socket2, message.c_str(), message.length(), 0);

        if(send_len == -1)
        {
            std::cerr <<"FFailed to send message to server." << std::endl;
            
            break;
        }

    }*/

    //close socket
    close(socket2);

    return 0;

}