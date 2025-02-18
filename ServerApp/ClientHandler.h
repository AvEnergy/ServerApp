#pragma once
#include "ChatServer.h"

void inputCommands(SOCKET sock, std::string buffer);
void helpCommand(SOCKET sock);