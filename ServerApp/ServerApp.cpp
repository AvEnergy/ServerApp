
#include <winsock2.h>
#include <Windows.h>
#include <iostream>

#include "ChatServer.h"

int main()
{
	startup();
	Server server;
	server.serverSetup();
	server.getServerInfo(std::to_string(server.inputPortNumber));
	server.init();
}
