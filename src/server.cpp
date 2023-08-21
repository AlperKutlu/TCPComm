#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <cstddef> 

using json = nlohmann::json;

struct slave1
{
    std::string slaveName;
    uint32_t vel;
    uint32_t pos;
    uint16_t statusWord;
};

enum class DataType{
    INT8,
    INT16
};

struct dataInfo{
    std::string dataName;
    DataType type;
};

std::string DataTypeToString(DataType type) {
    switch (type) {
        case DataType::INT8: return "int8";
        case DataType::INT16: return "int16";
        default: return "unknown";
    }
}

void HandleClient(int client_socket, const std::vector<dataInfo>& dataInfos) {
    std::cout << "Client connected." << std::endl;

    json data;

    for (unsigned int i = 0; i < dataInfos.size(); ++i) {
        const dataInfo& info = dataInfos[i];

        json rx;
        rx["type"] = DataTypeToString(info.type);

        json tx;

        json slave;
        slave["rx"] = rx;
        slave["tx"] = tx;

        data[info.dataName] = slave;
    }

    std::string json_string = data.dump(4); // Use 4 as the indentation for pretty printing

    if (send(client_socket, json_string.c_str(), json_string.size(), 0) == -1) {
        std::cerr << "Failed to send JSON data to client." << std::endl;
    } else {
        std::cout << "JSON data sent successfully." << std::endl;
    }

    close(client_socket);

    std::cout << "Client disconnected." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port_number> " << std::endl;
        return 1;
    }
    
    int port = std::stoi(argv[1]);
    
    

    std::vector<dataInfo> dataInfos = {
        {"slave", DataType::INT16},
        {"slave2", DataType::INT8}
    };


    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to bind socket." << std::endl;
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        std::cerr << "Failed to listen for incoming connections." << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Server is listening on port " << port << std::endl;

    for (int i = 0; i < 1000; ++i) {
        struct sockaddr_in client_address;
        socklen_t client_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_size);

        if (client_socket == -1) {
            std::cerr << "Failed to accept client connection." << std::endl;
            continue;
        }

        std::thread client_thread(HandleClient, client_socket, dataInfos);
        client_thread.detach();
    }

    close(server_socket);

    return 0;
}
