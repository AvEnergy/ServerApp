#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <iostream>

#pragma comment(lib,"Ws2_32.lib")
#include "stdint.h"
#include <set>
#include "WS2tcpip.h"
#include <string>
#include <cstring>
#include <stdlib.h>
#include "MessageHandler.h"

class Server
{

public:
	SOCKET listeningSocket;
	SOCKET connectedSocket;
	fd_set masterSet;
	fd_set readySet;
	
	uint16_t inputPortNumber;
	int capacity;
	char commandChar;

	void serverSetup();
	void getServerInfo(const std::string& port);
	int init();
	void serverRun();
};

inline void startup()
{
	WSADATA wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);
}

inline void shutdown()
{
	
}