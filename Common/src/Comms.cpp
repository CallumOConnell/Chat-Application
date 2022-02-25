#include "Comms.h"

bool Comms::Initialise()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	try
	{
		int wsaerr = WSAStartup(wVersionRequested, &wsaData);

		if (wsaerr != 0)
		{
			throw "The winsock dll was not found";
		}

		printf("Found the winsock dll\n");

		return true;
	}
	catch (char* error)
	{
		printf("Exception: %s\n", error);

		return false;
	}
}

SOCKET Comms::CreateSocket()
{
	SOCKET newSocket = INVALID_SOCKET;

	newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	return newSocket;
}