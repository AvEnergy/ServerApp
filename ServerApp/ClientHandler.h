#pragma once
#include "ChatServer.h"

void inputCommands(SOCKET sock, std::string buffer, std::string fullBuffer);
void helpCommand(SOCKET sock);
void splitString(std::string& str, std::string& first, std::string& second, std::string& third);
void registerCommand(SOCKET sock, std::string username, std::string password);