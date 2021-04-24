#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <experimental/filesystem>
#include <dirent.h>
#include <sys/select.h>
#include <sys/time.h>
#include "SSClient.h"
#include "SSnetworking.h"
#include "json.h"


//namespace fs = std::experimental::filesystem;
void sendOutSpreadsheets(SSClient* client);
int acceptNewConnection(int sockfd, struct sockaddr* address, int addrsize);
int processMessageFromClient(int sock);
SSClient * getClientBySocket(int sock);
SSnetworking * openSpreadsheet(char * request);
SSnetworking * getSpreadsheetFromVector(char * request);
char * trimNewline(char *value);

std::vector<SSnetworking*> spreadsheetVector;

SSnetworking defaultSS = SSnetworking("default");
fd_set connections;


int main()
{
	int selectReturn;

	fd_set readFDs, writeFDs, exceptFDs;
	struct timeval timeout;

	spreadsheetVector.push_back(&defaultSS);
	//defaultSS = SSnetworking("default");
	struct sockaddr_in address;
	int port = 1100;
	int maxFDs = 0;



	FD_ZERO(&connections);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	


	//add our server socket to the select wait bits
	FD_SET(sockfd, &connections);
	maxFDs = sockfd;
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);


	bind(sockfd, (struct sockaddr*)&address, sizeof(address));



	listen(sockfd, 10);

	
	int addrsize = sizeof(address);
	
	
	while (true)
	{
		std::cout<<"loop"<<std::endl;
		timeout.tv_sec = 3600;
		readFDs = connections;
		FD_ZERO(&writeFDs);
		exceptFDs = connections;
		selectReturn = select(FD_SETSIZE, &readFDs, &writeFDs, &exceptFDs, &timeout);
		// std::cout<<"5"<<std::endl;
	
		if (FD_ISSET(sockfd, &readFDs))
		{	
			//we have a new client to add
			int clientSock = acceptNewConnection(sockfd, (struct sockaddr*) &address, addrsize);

			if (clientSock >= 0)
			{
				//add to list of connections we need to listen for
				FD_SET(clientSock, &connections);
				if (clientSock > maxFDs)
				{
					maxFDs = clientSock;
				}
			}
			FD_CLR(sockfd, &readFDs);
		}
		
		//loop through the readFDs until all is set to 0

		//
		for (int i = 0; i <= maxFDs; i++)
		{

			if (FD_ISSET(i, &readFDs))
			{
				std::cout<<"prc"<<std::endl;
				int ret = processMessageFromClient(i);
				if (ret == -1)
				{
					//if we got a negative return value, client no longer connected.
					//remove from connections to select() for
					FD_CLR(i, &connections);	
				}
				FD_CLR(i, &readFDs);

			}

			//exceptions/ close connection
			if (FD_ISSET(i, &exceptFDs))
			{
				//deal with exception
				FD_CLR(i, &exceptFDs);
			}

					
		}	


	}
}

int processMessageFromClient(int sock)
{
	SSClient *client = getClientBySocket(sock);

	if (client == NULL)
	{
		abort();
	}

	//process Messages
	//read from the client
	char* request = client->readRequest();
    
    std::cout << request << std::endl;
	
	if (request == NULL)
	{
		//client closeed, remove them from SS, return error to caller
		return -1;
	}
	request = trimNewline(request);
	//process request
	if (client->getName().size()  == 0)
	{
		//the info we received is the name
		client->setName(request);

		sendOutSpreadsheets(client);

	}
	else if (client->getSpreadsheet() == &defaultSS)
	{
		//create the spreadsheet if it doesnt exist
		//for right now assume there is not one yet

		defaultSS.removeClient(client);

		//see if spreadsheet with this name already exists in the spreadsheetVector 
		//if so, add to the existing one. 
		//if NOT, open the spreadsheet, (or create if DNE) load contents into spreadsheet object
		//add to spreadsheetVector
		SSnetworking *addSpreadsheet = getSpreadsheetFromVector(request);	
		if ( addSpreadsheet == NULL)
		{
            std::cout<<"1"<<std::endl;
			addSpreadsheet = openSpreadsheet(request);
			if (addSpreadsheet == NULL)
			{
				abort();
			}
            std::cout<<"2"<<std::endl;
			spreadsheetVector.push_back(addSpreadsheet);
			std::cout<<"3"<<std::endl;
		}
		addSpreadsheet->addClient(client);
		std::cout<<"4"<<std::endl;
	}
	else {
	  std::cout << "type in test json" << std::endl;
		//process command
	  // Json::Value root;
	  //root << request;

	  // std::cout << root.get("requestType","NULL").asString();
	  
        sendOutSpreadsheets(client);

	}

	return 0;

}


char * trimNewline(char *value)
{
	if (value == NULL)
	{
		return NULL;
	}
	int len = strlen(value);
	if (value[len -1] == '\n')
	{
		value[len-1] = 0;
	}
	return value;
}

//get the spreadsheet if it exists in the vector, NULL if not
SSnetworking * getSpreadsheetFromVector(char * request)
{
		
	return NULL;
}

//open the spreadsheet if it exists in the folder of spreadsheets
//otherwise creates a new file and opens it
SSnetworking * openSpreadsheet(char * request)
{
	return new SSnetworking(request);
	return NULL;
}

//look in all the SSnetworking objects, and find the client with the same socket ID
SSClient * getClientBySocket(int sock)
{
	SSClient *client;
	SSnetworking *spreadsheet;
	for (int i = 0; i < spreadsheetVector.size(); i++)
	{
		spreadsheet = spreadsheetVector[i];
		for (int j = 0; j < spreadsheet->getNumClients(); j++)
		{

			client = spreadsheet->getClient(j);
			if (client->getSocket() == sock)
			{
				return client;
			}
		}	

	}		
	return NULL;
}




int acceptNewConnection(int sockfd, struct sockaddr* address, int addrsize)
{

	int newSocket = accept(sockfd, address, (socklen_t*)&(addrsize));

	//call other methods to set up connection
	SSClient *newClient = new SSClient(newSocket);
	
	defaultSS.addClient(newClient);
	return newSocket;

}


/*
* Read the spreadsheet directory, find all files in it, send a new line 
*	separated list of all the file names to the client. Add \n\n at end of list
*/
void sendOutSpreadsheets(SSClient* client)
{
	std::vector<std::string> files;

	//put spreadsheets in desired locations
	char* path = "./spreadsheets";
	
	//read directory, add them to vector, then sort the vector alphabetically
	//
	class dirent *ent;
	DIR *dir = opendir(path);
	if (dir != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if(ent->d_type == DT_REG)
			{
				files.push_back(ent->d_name);
			}
		}
	}
	closedir(dir);
	std:sort(files.begin(), files.end());

	char eol[] = "\n";
	//for each element on the vector send to the client followed by "\n",
	for (int i = 0; i < files.size(); i++)
	{
		client->writeResponse(&(files[i])[0]);
		client->writeResponse(eol);

	}

	//if directory is empty, need to send \n by itself so we get \n\n
	if (files.size() == 0)
	{
		client->writeResponse(eol);
	}
	client->writeResponse(eol);

}
