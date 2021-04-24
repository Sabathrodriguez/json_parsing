#pragma once
#include <string.h>
#include <string>
//#include "SSnetworking.h"
class SSnetworking;

class SSClient {

private:
	std::string userName;
	int socket;
	SSnetworking *spreadsheet;
	
	char *buffer;

public:
	SSClient(int socket);
	void clientClose();

	void setName(std::string name);
	std::string getName();

	int getSocket();

	void setSpreadsheet(SSnetworking* ss);
	SSnetworking *getSpreadsheet();

	//read
	char* readRequest();

	//write
	void writeResponse(char* c);
	
};
