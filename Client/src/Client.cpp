#include "Client.h"

bool Client::Setup()
{
	if (!Initialise()) return false;

	try
	{
		SOCKET temp = CreateSocket();

		if (temp == SOCKET_ERROR)
		{
			throw -1;
		}

		m_clientSocket = temp;

		printf("Successfully created client socket\n");

		return true;
	}
	catch (int errorCode)
	{
		printf("Failed to create client socket\n");

		return false;
	}
}

bool Client::ConnectToServer()
{
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &clientService.sin_addr);

	try
	{
		int connectionResult = connect(m_clientSocket, (SOCKADDR*)&clientService, sizeof(clientService));

		if (connectionResult == SOCKET_ERROR)
		{
			throw -1;
		}

		printf("Client successfully connected to the server\n");

		return true;
	}
	catch (int errorCode)
	{
		printf("Client failed to connect to server\n");
		WSACleanup();
		return false;
	}
}

void Client::Run()
{
	const int bufferSize = 200;

	char buffer[bufferSize] = "";

	std::string userInput;

	while (true)
	{
		// Prompt user to input text
		printf("Client> ");

		// Put user input into userInput string
		std::getline(std::cin, userInput);

		// Exit loop if client enters quit
		if (userInput == "quit") break;

		// Check to see if the user has actually typed something
		if (userInput.size() > 0)
		{
			// Send userInput to server
			int sendResult = send(m_clientSocket, userInput.c_str(), userInput.size(), 0);

			if (sendResult == SOCKET_ERROR)
			{
				printf("Send failed with error: %d\n", WSAGetLastError());
			}

			// Wait for a response
			int bytesReceived = recv(m_clientSocket, buffer, bufferSize, 0);
			
			if (bytesReceived > 0)
			{
				std::cout << "Server> " << std::string(buffer, 0, bytesReceived) << std::endl;
			}
			else if (bytesReceived == 0 || bytesReceived == WSAECONNRESET)
			{
				printf("Connection closed with server\n");
				break;
			}
			else
			{
				printf("Recv failed with error: %d\n", WSAGetLastError());
				break;
			}
		}
	}

	// Close socket
	closesocket(m_clientSocket);

	// Cleanup winsock
	WSACleanup();
}