#include "ClientHandler.h"
#include "Logger.h"

void inputCommands(SOCKET sock, std::string buffer, std::string fullBuffer)
{
	if (buffer == "help" || buffer ==  "Help")
	{
		helpCommand(sock);
	}
	else if (buffer == "register" || buffer == "Register")
	{
		std::string username, password;
		splitString(fullBuffer, buffer, username, password);
		registerCommand(sock, username, password);
	}
	else if (buffer == "login" || buffer == "Login")
	{
		std::string username, password;
		splitString(fullBuffer, buffer, username, password);
		loginCommand(sock, username, password);
	}
	else if (buffer == "send" || buffer == "Send")
	{
		//sendCommand(sock);
	}
	else if (buffer == "getlist" || buffer == "Getlist")
	{
		//getlistCommand(sock);
	}
	else if (buffer == "getlogs" || buffer == "Getlogs")
	{
		//getlogsCommand(sock);
	}
	else if (buffer == "exit" || buffer == "Exit")
	{
		//exitCommand(sock);
	}
	else
	{
		std::cout << sock <<"sent Invalid command" << std::endl;
		const char message[] = "Invalid Command input";
		SendSingleMessage(sock, (char*)message, sizeof(message));
	}
}

void helpCommand(SOCKET sock)
{
	const char helpMessage1[] = "Commands: help - displays this message.";
	const char helpMessage2[] = "register Username Password - registers user to server.";
	const char helpMessage3[] = "login Username Password - Logs User in.";
	const char helpMessage4[] = "send RecipientUserName MessageText - Sends a private message to user.";
	const char helpMessage5[] = "getlist - gets a list of active users.";
	const char helpMessage6[] = "getlogs - gets chat history.";
	const char helpMessage7[] = "exit - disconnects user.";
	
	SendSingleMessage(sock, (char*)helpMessage1, sizeof(helpMessage1));
	SendSingleMessage(sock, (char*)helpMessage2, sizeof(helpMessage2));
	SendSingleMessage(sock, (char*)helpMessage3, sizeof(helpMessage3));
	SendSingleMessage(sock, (char*)helpMessage4, sizeof(helpMessage4));
	SendSingleMessage(sock, (char*)helpMessage5, sizeof(helpMessage5));
	SendSingleMessage(sock, (char*)helpMessage6, sizeof(helpMessage6));
	SendSingleMessage(sock, (char*)helpMessage7, sizeof(helpMessage7));
}

void registerCommand(SOCKET sock, std::string username, std::string password)
{
	if (RegisterMap.find(username) != RegisterMap.end())
	{
		const char message[] = "Username already exists.";
		SendSingleMessage(sock, (char*)message, sizeof(message));
		return;
	}
	else if (username == "register" || username == "Register")
	{
		const char message[] = "Username and password required. EX: /register Username password";
		SendSingleMessage(sock, (char*)message, sizeof(message));
		return;
	}
	else if (username == password)
	{
		const char message[] = "Username and password must be different. EX: /register Username password";
		SendSingleMessage(sock, (char*)message, sizeof(message));
		return;
	}
	else
	{
		RegisterMap[username] = password;
		const char message[] = "User registered.";
		SendSingleMessage(sock, (char*)message, sizeof(message));
	}

}

void splitString(std::string& str, std::string& first, std::string& second, std::string& third)
{
	std::string::size_type pos = str.find(' ');
	first = str.substr(0, pos);
	str = str.substr(pos + 1);
	pos = str.find(' ');
	second = str.substr(0, pos);
	third = str.substr(pos + 1);
}

void loginCommand(SOCKET sock, std::string username, std::string password)
{
	//Check if username is in RegisterMap
	if (RegisterMap.find(username) == RegisterMap.end())
	{
		const char message[] = "Username not registered.";
		std::cout << message << std::endl;
		SendSingleMessage(sock, (char*)message, sizeof(message));
		return;
	}
	//check if password matches username
	else if (RegisterMap[username] != password)
	{
		const char message[] = "Invalid password.";
		std::cout << message << std::endl;
		SendSingleMessage(sock, (char*)message, sizeof(message));
		return;
	}
	else
	{
		UserMap[sock] = username;
		const char message[] = "User logged in.";
		std::cout << message << std::endl;
		SendSingleMessage(sock, (char*)message, sizeof(message));
	}

}