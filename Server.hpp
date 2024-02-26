#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <cstdlib>
#include <arpa/inet.h>
#include <cstdint>
#include <vector>
#include "Log.hpp"

#define ERROR "\033[1;31m[ERROR]\033[0m "
#define INFO "\033[1;32m[INFO]\033[0m "

class Server
{
private:
    sockaddr_in serverAddress;
    int max_client;
    int serverSocket;
    int max_threads;
    bool start;
    Log log;
    std::vector<std::vector<int>> clients;
    std::vector<std::thread> threads;
    int FreeThread()
    {
        int min = clients[0].size();
        int number_thread = 0;
        for(int i = 1; i < max_threads; i++)
        {
            if (min > clients[i].size())
                {
                    min = clients[i].size();
                    number_thread = i;
                }
        }
        return number_thread;
    }
public:
    Server(int type_connection=SOCK_STREAM, int ip_version=AF_INET, int protocol=0, std::string ip="127.0.0.1", 
            std::string port="8700", int max_client=SOMAXCONN, std::string path_log="", std::string log_name="unknown.txt", int max_threads=std::thread::hardware_concurrency())
    {
        log = Log(path_log, log_name);
        serverSocket = socket(ip_version, type_connection, protocol);
        if (serverSocket < 0)
        {
            log.write(std::string().append(ERROR).append("Socket don't open."));
            exit(-1);
        }
        serverAddress.sin_family = ip_version;
        serverAddress.sin_port = htons(atoi(port.c_str()));
        serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
        this->max_threads = max_threads;
        if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        {
            log.write(std::string().append(ERROR).append("Failed bind."));
            exit(-1);
        }
        if (max_client <= SOMAXCONN)
        {
            this->max_client = max_client;
        }
        else
        {
            max_client = SOMAXCONN;
        }
        clients.reserve(max_threads);
        threads.reserve(max_threads);
    }

    void Start(void(*action)(std::vector<int>* clients, bool* status_server))
    {
        log.write(std::string().append(INFO).append("Server start..."));
        start = true;
        for (int i = 0; i < max_threads; i++)
        {
            threads.push_back(std::thread([&action](std::vector<int>* clients, bool* status_server)
            {        
                action(clients, status_server);
            }, &clients[i], &start));
        }
        for (int i = 0; i < max_threads; i++)
        {
            threads[i].detach();
        }        
        for(;start;)
        {
            if (listen(serverSocket, max_client))
            {
                log.write(std::string().append(ERROR).append("Failed listen."));
                exit(-1);
            }
            int clientSocket = accept(serverSocket, NULL, NULL);
            if (clientSocket < 0)
            {
                log.write(std::string().append(ERROR).append("Client try connected."));
            }
            clients[FreeThread()].push_back(clientSocket);
        }        
    }

    void Stop()
    {
        start = false;     
        shutdown(serverSocket, SHUT_RDWR); 
        close(serverSocket);        
        for (int i = 0; i < max_threads; i++)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
        threads.clear();
        clients.clear();
        serverAddress = sockaddr_in();
        max_client = 0;
        serverSocket = 0;
        max_threads = 0;
        log.~Log();
    }
};
