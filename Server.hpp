#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "Log.hpp"

#define ERROR "\033[1;31m[ERROR]\033[0m "
#define INFO "\033[1;32m[INFO]\033[0m "

class Server
{
private:
    sockaddr_in serverAddress;
    int max_client;
    int serverSocket;
    int max_thread;
    bool FreeThread(std::atomic_ulong* count_thread)
    {
        for(;;)
        {
            if (*count_thread < max_thread)
                return true;
        }
    }
public:
    Server(int type_connection=SOCK_STREAM, int ip_version=AF_INET, int protocol=0, std::string ip="127.0.0.1",
           std::string port="8700", int max_client=SOMAXCONN, std::string path_log="unknown", std::string log_name="unknown", int max_threads=2)
    {
        serverSocket = socket(ip_version, type_connection, protocol);
        serverAddress.sin_family = ip_version;
        serverAddress.sin_port = htons(atoi(port.c_str()));
        serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
        this->max_thread = max_thread;
        bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
        if (max_client <= SOMAXCONN)
        {
            this->max_client = max_client;
        }
        else
        {
            max_client = SOMAXCONN;
        }
    }

    void Start(void(*action)(int*))
    {
        std::atomic_ulong count_thread = std::atomic_ulong(0);
        for(;;)
        {
            listen(serverSocket, SOMAXCONN);
            int clientSocket;
            clientSocket = accept(serverSocket, nullptr, nullptr);
            if (FreeThread(&count_thread))
            {
                std::thread([&action, &count_thread, &clientSocket](){
                    count_thread++;
                    action(&clientSocket);
                    count_thread--;
                    close(clientSocket);
                }).detach();
            }
        }
    }

    ~Server()
    {
        close(serverSocket);
    }
};
