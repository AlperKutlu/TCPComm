#include "/home/alper/git/TCPComm/include/TCPComm/server.hpp"





char msg[500];

SocketComm::SocketComm()
{
    m_Socket = socket(AF_INET, SOCK_STREAM, 0);

    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_addr.s_addr = INADDR_ANY;
    m_ServerAddress.sin_port = htons(PORT);
    bind(m_Socket, (struct sockaddr *)&m_ServerAddress, sizeof(m_ServerAddress));
}

SocketComm::~SocketComm()
{
    
}

int SocketComm::GetSocket(){
    return m_Socket;
}

bool SocketComm::init()
{
 
}

void SocketComm::ClientComm(int client_socket)
{
    // send data to the client
    const char *message = "Hello, client!";
    send(client_socket, message, strlen(message), 0);

    // receive data from the client
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    std::cout << "Client message: " << buffer << std::endl;

}



int main(int argc, char *argv[]) {
    SocketComm ab;

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