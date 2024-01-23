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
