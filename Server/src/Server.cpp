#include "Server.h"

bool Server::Setup()
{
	if (!Initialise()) return false;

	try
	{
		SOCKET temp = CreateSocket();

		if (temp == INVALID_SOCKET)
		{
			throw ~0;
		}

		if (temp == SOCKET_ERROR)
		{
			throw -1;
		}

		m_serverSocket = temp;

		printf("Successfully created server socket\n");
	}
	catch (int errorCode)
	{
		printf("Failed to create server socket with error code %d\n", errorCode);

		return false;
	}

	if (!BindSocket()) return false;
	if (!ListenSocket()) return false;
	if (!AcceptConnection()) return false;

	return true;
}

bool Server::BindSocket()
{
	sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	serverService.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &serverService.sin_addr);

	try
	{
		int bindResult = bind(m_serverSocket, (SOCKADDR*)&serverService, sizeof(serverService));

		if (bindResult == SOCKET_ERROR)
		{
			throw -1;
		}

		printf("Successfully binded server socket\n");

		return true;
	}
	catch (int errorCode)
	{
		printf("Bind failed with error: %d\n", WSAGetLastError());
		closesocket(m_serverSocket);
		WSACleanup();
		return false;
	}
}

bool Server::ListenSocket()
{
	try
	{
		int listenResult = listen(m_serverSocket, 1);

		if (listenResult == SOCKET_ERROR)
		{
			throw -1;
		}

		printf("Server socket is now listening for connections\n");

		return true;
	}
	catch (int errorCode)
	{
		printf("Listen failed with error: %d\n", WSAGetLastError());

		return false;
	}
}

bool Server::AcceptConnection()
{
	m_clientSize = sizeof(m_client);

	try
	{
		m_clientSocket = accept(m_serverSocket, (SOCKADDR*)&m_client, &m_clientSize);

		if (m_clientSocket == INVALID_SOCKET)
		{
			throw -1;
		}

		m_returnValue = getnameinfo((SOCKADDR*)&m_client, m_clientSize, m_host, NI_MAXHOST, m_service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		if (m_returnValue != 0)
		{
			throw;
		}

		printf("Accepted connection from host %s and port %s\n", m_host, m_service);

		closesocket(m_serverSocket);

		return true;
	}
	catch (int errorCode)
	{
		printf("Accept failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	catch (...)
	{
		printf("getnameinfo failed with error: %d\n", m_returnValue);
		WSACleanup();
		return false;
	}
}

void Server::Run()
{
	int const bufferSize = 200;

	char buffer[bufferSize] = "";

	std::string userInput;

	while (true)
	{
		int bytesReceived = recv(m_clientSocket, buffer, bufferSize, 0);

		if (bytesReceived > 0)
		{
			std::cout << "Client> " << std::string(buffer, 0, bytesReceived) << std::endl;

			// Prompt user to input text
			printf("Server> ");

			// Put user input into userInput string
			std::getline(std::cin, userInput);

			// Check to see if the user actually typed something
			if (userInput.size() > 0)
			{
				// Echo the buffer back to the client
				int bytesSent = send(m_clientSocket, userInput.c_str(), userInput.size(), 0);

				if (bytesSent == SOCKET_ERROR)
				{
					printf("Send failed with error: %d\n", WSAGetLastError());
					break;
				}
			}
		}
		else if (bytesReceived == 0 || bytesReceived == SOCKET_ERROR)
		{
			printf("Connection closing...\n");
			break;
		}
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			break;
		}
	}

	// Close socket
	closesocket(m_clientSocket);

	// Cleanup winsock
	WSACleanup();
}