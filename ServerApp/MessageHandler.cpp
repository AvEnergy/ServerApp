#include "MessageHandler.h"
#include "ClientHandler.h"

int ReadMessage(SOCKET sock, FD_SET masterSet, char commandChar)
{
	char buffer[255];
	int byteSiz = 0;
	int result = recv(sock, (char*)&byteSiz, 1, 0);
	if (result == -1)
		return -1;

	int total = 0;
	while (total < byteSiz)
	{
		int result = recv(sock, buffer + total, byteSiz - total, 0);
		std::cout <<"From user "<< sock << " : " << buffer << std::endl;
		if (buffer[0] == commandChar)
		{	
			std::string token = buffer;
			token.erase(0, 1);
			token.substr(0, token.find(' '));
			inputCommands(sock, token);
		}
		else
		{
			SendToAllMessage(sock, buffer, masterSet, byteSiz);
		}

		total += result;
	}
	
}

int SendToAllMessage(SOCKET sock, char* buffer, FD_SET masterSet, int size)
{
	for (int i = 1; i < masterSet.fd_count; i++)
	{
		SOCKET currentS = masterSet.fd_array[i];
		if (currentS == sock)
			continue;
		send(currentS, (char*)&size, 1, 0);
		send(currentS, buffer, strlen(buffer) + 1, 0);
	}
	return 0;
}

int SendSingleMessage(SOCKET sock, char* buffer, int size)
{
	send(sock, (char*)&size, 1, 0);
	send(sock, buffer, strlen(buffer) + 1, 0);
	return 0;
}

int SendWelcomeMessage(SOCKET sock, char commandChar)
{

	const char welcome[28] = "Welcome to the chat server!";
	SendSingleMessage(sock, (char*)welcome, 28);

    char commandMessage[21];
    snprintf(commandMessage, sizeof(commandMessage), "Command character: %c", commandChar);
	SendSingleMessage(sock, commandMessage, 21);

	return 0;
}


