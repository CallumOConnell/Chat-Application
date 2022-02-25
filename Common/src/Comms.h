#pragma once

#include "Core.h"

class Comms
{
public:
	const std::string ipAddress = "127.0.0.1";
	const int port = 55555;

	virtual bool Setup() = 0;
	bool Initialise();
	SOCKET CreateSocket();
};