#pragma once
#include "ChatServer.h"

void inputCommands(SOCKET sock, std::string buffer, std::string fullBuffer, FD_SET masterSet);
void helpCommand(SOCKET sock);
void splitString(std::string& str, std::string& first, std::string& second, std::string& third);
void registerCommand(SOCKET sock, std::string username, std::string password);
void loginCommand(SOCKET sock, std::string username, std::string password);
void getlistCommand(SOCKET sock);
bool isUserLoggedin(SOCKET sock, std::string username);
void sendCommand(SOCKET sock, std::string username, std::string message);
void getlogsCommand(SOCKET sock);
void exitCommand(SOCKET sock, FD_SET masterSet);