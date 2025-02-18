#pragma once
#include "ChatServer.h"

int ReadMessage(SOCKET sock, FD_SET masterSet, char commandChar);
int SendToAllMessage(SOCKET sock, char* buffer, FD_SET masterSet, int size);
int SendSingleMessage(SOCKET sock, char* buffer, int size);
int SendWelcomeMessage(SOCKET sock, char commandChar);

