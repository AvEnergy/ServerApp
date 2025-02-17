#include "MessageHandler.h"


int ReadMessage(SOCKET sock)
{
	char buffer[256];
	int byteRead = recv(sock, buffer, sizeof(buffer), 0);
	if (byteRead <= 0)
	{
		std::cout << "Client disconnected: " << sock << std::endl;
		return -1;
	}
	else
	{
		int msgLength = static_cast<unsigned char>(buffer[0]);
		std::string message(buffer + 1, msgLength);
		std::cout << "Received from " << sock << " : " << message << std::endl;
	}
}


