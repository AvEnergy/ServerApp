#include "ChatServer.h"

void Server::serverSetup()
{
	std::set<char> validChars = { '/', '!', '@' , '~' , '>' , '$' , '#' };
	int portNumber, chatCapacity;
	char specialChar = '/';

	std::cout << "Welcome, let's set up the parameters for the server." << std::endl;
	while (true)
	{
		std::cout << "Enter port number for the server: ";
		std::cin >> portNumber;
		if (portNumber == 123) //This is for quick server setup with defualt values, allows for quicker debugging.
		{
			this->capacity = 5;
			this->commandChar = '/';
			this->inputPortNumber = 31337;
			return;
		}
		//Add some verification that port number is valid.
		std::cout << "Port: " << portNumber << std::endl;
		break;
	}
	while (true)
	{
		std::cout << "Enter the max capacity for the server: ";
		std::cin >> chatCapacity;
		if (chatCapacity > 10 || chatCapacity < 1)
		{
			std::cout << "Error: Chat capacity is out of range. (Between 1 - 10)" << std::endl;
		}
		else
		{
			std::cout << "Chat Capacity: " << chatCapacity << std::endl;
			break;
		}
	}
	while (true)
	{
		std::cout << "Enter Command Character. (Default is /) : ";
		std::cin >> specialChar;
		if (validChars.find(specialChar) == validChars.end())
		{
			std::cout << "Invalid command character. Pick one of the following: / ! @ ~ > $ #" << std::endl;
			std::cin.clear();
		}
		else
		{
			std::cout << "Command Character: " << specialChar << std::endl;
			break;
		}
	}

	this->capacity = chatCapacity;
	this->commandChar = specialChar;
	this->inputPortNumber = portNumber;
	system("cls");
	std::cout << "Port: " << portNumber << std::endl;
	std::cout << "Chat Capacity: " << chatCapacity << std::endl;
	std::cout << "Command Character: " << specialChar << std::endl;
}

void Server::getServerInfo(const std::string& port)
{
	char hostname[256];
	gethostname(hostname, sizeof(hostname));
	struct addrinfo hints{}, *res;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo(hostname, port.c_str(), &hints, &res);

	std::cout <<"Hostname: " << hostname << std::endl;
	while (res)
	{
		char ipStr[INET6_ADDRSTRLEN];
		void* addr;
		if (res->ai_family == AF_INET) {
			addr = &((struct sockaddr_in*)res->ai_addr)->sin_addr;
		}
		else {
			addr = &((struct sockaddr_in6*)res->ai_addr)->sin6_addr;
		}
		inet_ntop(res->ai_family, addr, ipStr, sizeof(ipStr));
		std::cout << "Listening on: " << ipStr << ":" << port << std::endl;
		res = res->ai_next;
	}
	freeaddrinfo(res);
}

int Server::init()
{
	listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listeningSocket == INVALID_SOCKET)
	{
		std::cout << "Listening Socket not created." << std::endl;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = INADDR_ANY;
	server.sin_port = htons(this->inputPortNumber);
	if (bind(listeningSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::cout << "Listening Socket binding error." << std::endl;
	}
	if (listen(listeningSocket, this->capacity) == SOCKET_ERROR)
	{
		std::cout << "Listening Socket setup error." << std::endl;
	}

	FD_ZERO(&masterSet);
	FD_SET(listeningSocket, &masterSet);
	FD_ZERO(&readySet);

	serverRun();
	//return success
	return 0;
}

void Server::serverRun()
{
	while (true)
	{
		readySet = masterSet;
		int readyFD = select(0, &readySet, NULL, NULL, NULL);

		for (int i = 0; i < readySet.fd_count; i++)
		{
			SOCKET currentS = readySet.fd_array[i];
			if (currentS == listeningSocket)
			{
				currentS = accept(listeningSocket, NULL, NULL);
				std::cout << currentS << " connected." << std::endl;
				FD_SET(currentS, &masterSet);
				SendWelcomeMessage(currentS, this->commandChar);
			}
			else
			{
				if (ReadMessage(currentS, masterSet, commandChar) == -1)
				{
					std::cout << currentS << " disconnected." << std::endl;
					closesocket(currentS);
					FD_CLR(currentS, &masterSet);
				}
			}
		}
	}
}