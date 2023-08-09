#include "/home/alper/git/TCPComm/include/TCPComm/server.hpp"



char msg[500];

SocketComm::SocketComm()
{

}

SocketComm::~SocketComm()
{
    
}

bool SocketComm::init(){
    m_Socket = socket(AF_INET, SOCK_STREAM, 0);

    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_addr.s_addr = INADDR_ANY;
    m_ServerAddress.sin_port = htons(m_PortNum);
    std::cout << m_PortNum;
    bind(m_Socket, (struct sockaddr *)&m_ServerAddress, sizeof(m_ServerAddress));
}

int SocketComm::GetSocket(){
    return m_Socket;
}

void SocketComm::SetPort(int PortNum){
    m_PortNum = PortNum;
}


void SocketComm::ClientComm(int client_socket)
{
    const char *message = "Hello, client!";
    send(client_socket, message, strlen(message), 0);

    while (true)
    {
        char buffer[1024] = {0};
        int bytesRead = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0)
        {
            std::cout << "Received from client: " << buffer << std::endl;

            // Send a response 
            const char *response = "Received your message!";
            send(client_socket, response, strlen(response), 0);
        }
        else if (bytesRead == 0)
        {
            std::cout << "Client disconnected." << std::endl;
            break; 
        }
    }

    close(client_socket);
}



int main(int argc, char *argv[]) {
    int server_port;
    try {
        YAML::Node config = YAML::LoadFile("config.yml");

        std::string server_ip = config["server_ip"].as<std::string>();
        server_port = config["server_port"].as<int>();

        std::cout << "Server IP: " << server_ip << std::endl;
        std::cout << "Server Port: " << server_port << std::endl;
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
        return 1;
    }


    SocketComm ab;
    ab.SetPort(server_port);
    ab.init();
    listen(ab.GetSocket(), 3);
    while (1)
    {
        int client_socket;
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        client_socket = accept(ab.GetSocket(), (struct sockaddr *)&client_address, &addrlen);

        // Create a new thread for each client connection
        std::thread client_thread(&SocketComm::ClientComm, &ab, client_socket);
        client_thread.detach(); // Detach the thread to allow it to run independently
    }



    return 0;
}