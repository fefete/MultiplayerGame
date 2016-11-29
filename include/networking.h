#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DEFAULT_PORT "53000"
#define DEFAULT_BUFLEN 1204

#pragma comment(lib, "Ws2_32.lib")

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "types.h"



bool initWS();
int connect(SOCKET* s, int ai_family, int ai_socktype, int ai_protocol, const char* IP);
int sendTCP(SOCKET s, const char* buff, netPackage p);
int recieveTCP(SOCKET s, netPackage* p);