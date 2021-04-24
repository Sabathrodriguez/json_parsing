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
	return name;
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


std::string cellToJson(std::string location, std::string contents)
{
	char buffer[256];
	
	sprintf(buffer, "{\"messageType\": \"cellUpdated\", \"cellName\": \"%s\", \"contents\": \"%s\"}", location, contents); 
	return buffer;
}

char* userSelectToJson(char *location, int ID, char *name)
{
	char *buffer;
	sprintf(buffer, "{\"messageType\": \"cellSelected\", \"cellName\": \"%s\", \"selector\": \"%d\", \"selectorName\": \"%s\"}", location, ID, name);
	return buffer;
}

char *userDisconnected(int ID)
{
	char *buffer;
	sprintf(buffer, "{\"messageType\": \"disconnected\", \"user\": \"%d\"}", ID);
	return buffer;
}



void SSnetworking::sendSpreadsheetToClient(SSClient *client)
{

	//std::string location = "A2";
	//std::string contents = "Yeet";
	//std::string buffer = cellToJson(location, contents);

	//client->writeResponse(buffer);
	
	char buffer[256];
	char quote = '"';
	//sprintf(buffer, "{%cmessageType%c: %ccellUpdated%c, %ccellName%c: %cA2%c, %ccontents%c: %cHello%c}", 
	//		quote, quote, quote, quote, quote, quote, quote, quote, quote, quote, quote, quote);
	//sprintf(buffer, "{\"messageType\": \"cellUpdated\", \"cellName\": \"C3\", \"contents\": \"Hello\", \"message\": \"\", \"selector\": \"\", \"selectorName\": \"\"}");
	sprintf(buffer, "{\"messageType\": \"cellUpdated\", \"cellName\": \"C3\", \"contents\": \"Hello\"}"); 
	client->writeResponse(buffer);
	char buffer2[256];
	sprintf(buffer2, "{\"messageType\": \"cellUpdated\", \"cellName\": \"A1\", \"contents\": \"2\"}"); 
	client->writeResponse(buffer2);
	char buffer3[256];
	sprintf(buffer3, "{\"messageType\": \"cellUpdated\", \"cellName\": \"A2\", \"contents\": \"3\"}"); 
	client->writeResponse(buffer3);
	char buffer4[256];
	sprintf(buffer4, "{\"messageType\": \"cellUpdated\", \"cellName\": \"A3\", \"contents\": \"=A1 + A2\"}"); 
	client->writeResponse(buffer4);
	
}
void SSnetworking::sendUserLocationsToClient(SSClient *client)
{

	char buffer[256];
	char quote = '"';
//	sprintf(buffer, "{messageType: %ccellSelected%c, cellName: %cA5%c, selector: %c100%c, selectorName: %cBigDaddy%c}", 
//			quote, quote, quote, quote, quote, quote, quote, quote);
	//sprintf(buffer, "{\"messageType\": \"cellSelected\", \"cellName\": \"A5\", \"selector\": \"100\", \"selectorName\": \"BigDaddy\"}");
	//client->writeResponse(buffer);
}
void SSnetworking::sendUIDToClient(SSClient *client)
{
	
	char buffer[32];
       sprintf(buffer, "%d\n", client->getSocket());	
       //client->writeResponse(buffer);
}

bool SSnetworking::processIO(int FD)
{
	return false;
}
