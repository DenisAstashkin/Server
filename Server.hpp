#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdint.h>

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
};
