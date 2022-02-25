#pragma once

#include "Comms.h"

class Server : public Comms
{
public:
	int m_clientSize = 0;
	int m_returnValue = 0;

	char m_service[NI_MAXSERV] = ""; // Service (i.e. port) the client is connected on
	char m_host[NI_MAXHOST] = ""; // Client's remote name

	SOCKET m_serverSocket = INVALID_SOCKET;
	SOCKET m_clientSocket = INVALID_SOCKET;

	SOCKADDR_STORAGE m_client;

	bool Setup();
	bool BindSocket();
	bool ListenSocket();
	bool AcceptConnection();

	void Run();
};