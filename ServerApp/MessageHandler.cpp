#include "MessageHandler.h"
#include "ClientHandler.h"
#include "Logger.h"

int ReadMessage(SOCKET sock, FD_SET masterSet, char commandChar)
{
	char buffer[255];
	int byteSiz = 0;
	int result = recv(sock, (char*)&byteSiz, 1, 0);
	if (result == SOCKET_ERROR)
	{
		return -1;
	}

	int total = 0;
	while (total < byteSiz)
	{
		int result = recv(sock, buffer + total, byteSiz - total, 0);
		if (result == SOCKET_ERROR)
		{
			std::cout << "Failed to receive message" << std::endl;
			return -1;
		}
		std::cout <<"From user "<< sock << " : " << buffer << std::endl;
		if (buffer[0] == commandChar)
		{	
			std::string token = buffer;
			token.erase(0, 1);
			std::string keyCommand = token.substr(0, token.find(' '));
			inputCommands(sock, keyCommand, token, masterSet);
			break;
		}
		else
		{
			if (SendToAllMessage(sock, buffer, masterSet, byteSiz) == -1)
			{
				std::cout << "Failed to send message to all clients" << std::endl;
				return -1;
			}
		}

		total += result;
	}
	return 0;
}

int SendToAllMessage(SOCKET sock, char* buffer, FD_SET masterSet, int size)
{
	if (UserMap.find(sock) == UserMap.end())
	{
		std::cout << "User tried to send message without logging in." << std::endl;

		const char message[] = "Message not sent. Login required to send messages.";
		SendSingleMessage(sock, (char*)message, sizeof(message));
		return 0;
	}

	for (int i = 1; i < masterSet.fd_count; i++)
	{
		SOCKET currentS = masterSet.fd_array[i];
		if (currentS == sock) //Don't send message to self.
			continue;
		if (UserMap.find(currentS) == UserMap.end())//If the reciving user is not logged in they will be skipped. (No message displays for user.)
			continue;

		if (send(currentS, (char*)&size, 1, 0) == SOCKET_ERROR)
		{
			std::cout << "Failed to send message size to client" << std::endl;
			return -1;
		}

		if (send(currentS, buffer, strlen(buffer) + 1, 0) == SOCKET_ERROR)
		{
			std::cout << "Failed to send message to client" << std::endl;
			return -1;
		}
		else
		{
            std::ofstream chatLog("chatlog.txt", std::ios_base::app);
            if(chatLog.is_open())
            chatLog << UserMap[sock] << ": " << buffer << std::endl;
            chatLog.close();
		}
	}
	return 0;
}

int SendSingleMessage(SOCKET sock, char* buffer, int size)
{
	if (send(sock, (char*)&size, 1, 0) == SOCKET_ERROR)
	{
		std::cout << "Failed to send message size to client" << std::endl;
		return -1;
	}

	if (send(sock, buffer, strlen(buffer) + 1, 0) == SOCKET_ERROR)
	{
		std::cout << "Failed to send message to client" << std::endl;
		return -1;
	}
	return 0;
}

int SendWelcomeMessage(SOCKET sock, char commandChar)
{

	const char welcome[28] = "Welcome to the chat server!";
	if (SendSingleMessage(sock, (char*)welcome, 28) == -1)
	{
		std::cout << "Failed to send welcome message" << std::endl;
		return -1;
	}

    char commandMessage[21];
    snprintf(commandMessage, sizeof(commandMessage), "Command character: %c", commandChar);
	if (SendSingleMessage(sock, commandMessage, 21) == -1)
	{
		std::cout << "Failed to send command character message" << std::endl;
		return -1;
	}

	return 0;
}


