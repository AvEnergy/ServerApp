#include "ClientHandler.h"

void inputCommands(SOCKET sock, std::string buffer)
{
	if (buffer == "help")
	{
		helpCommand(sock);
	}
	else
	{
		std::cout << "Invalid command" << std::endl;
	}
}

void helpCommand(SOCKET sock)
{
	const char helpMessage[312] = "Commands: help - displays this message. \n register Username Password - registers user to server. \n login Username Password - Logs User in. \n send RecipientUserName MessageText - Sends a private message to user. \n getlist - gets a list of active users. \n getlogs - gets chat history. \n exit - disconnects user.";
	SendSingleMessage(sock, (char*)helpMessage, 312);
}