#include "SSnetworking.h"



int SSnetworking::getNumClients()
{
	return clients.size();
}
SSClient * SSnetworking::getClient(int index)
{
	if (index > clients.size())
	{
		return NULL;
	}
	return clients[index];
}

void SSnetworking::getRequestFromClient()
{
}

void SSnetworking::sendInfoToClient()
{
}

void SSnetworking::processHandShake()
{
}

void SSnetworking::processCommand()
{
}

void SSnetworking::replyToClient(SSClient c, char* message)
{
	c.writeResponse(message);
}

void SSnetworking::replyToAllClients(char* message)
{

	for (int i = 0; i < clients.size(); i++)
	{
		clients[i]->writeResponse(message);

	}
}

SSnetworking::SSnetworking(char* ssname)
{
	std::string result(ssname);
	this->name = result;
}

std::string SSnetworking::getName()
{
	return std::string();
}

void SSnetworking::addClient(SSClient *c)
{
	c->setSpreadsheet(this);
	clients.push_back(c);
}

void SSnetworking::removeClient(SSClient *c)
{

	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i] == c)
		{
			clients.erase(clients.begin() + i);
			break;
		}
	}

	
}

void SSnetworking::handleHandShake()
{
}

bool SSnetworking::processIO(int FD)
{
	return false;
}
