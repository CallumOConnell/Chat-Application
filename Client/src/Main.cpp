#include "Client.h"

int main()
{
	SetConsoleTitleA("Client");

	Client client;

	if (client.Setup())
	{
		if (client.ConnectToServer())
		{
			client.Run();
		}
	}

	return 0;
}