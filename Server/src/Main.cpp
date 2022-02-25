#include "Server.h"

int main()
{
	SetConsoleTitleA("Server");

	Server server;

	if (server.Setup())
	{
		server.Run();
	}

	return 0;
}