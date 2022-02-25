#pragma once

#include "Comms.h"

class Client : public Comms
{
public:
	SOCKET m_clientSocket = INVALID_SOCKET;

	bool Setup();
	bool ConnectToServer();
	void Run();
};