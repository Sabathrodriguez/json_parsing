#pragma once
#include <string.h>
#include <string>
#include <list>
#include <vector>
#include "SSClient.h"

class SSnetworking {



private:
	std::string name;
	// ssObject *spreadsheet;
	std::vector<SSClient*> clients;

	void getRequestFromClient();

	void sendInfoToClient();

	void processHandShake();
	void processCommand();

	void replyToClient(SSClient c, char* message);

	void replyToAllClients(char* message);
	
	std::string cellToJson(std::string location, std::string contents);
	char *userDisconnected(int ID);
	char* userSelectToJson(char *location, int ID, char *name);

public:
	SSnetworking(char* ssname);

	std::string getName();

	void sendSpreadsheetToClient(SSClient *client);
	void sendUserLocationsToClient(SSClient *client);
	void sendUIDToClient(SSClient *client);
	void addClient(SSClient *c);
	void removeClient(SSClient *c);

	//SSObject getSSObject();
	//void setSSObject(SSObject *spreadsheet)

	void handleHandShake();

	bool processIO(int FD);
	int getNumClients();
	SSClient * getClient(int index);

};
