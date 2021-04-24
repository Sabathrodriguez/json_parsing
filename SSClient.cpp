
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "SSClient.h"
#include "SSnetworking.h"


SSClient::SSClient(int socket)
{
	this->buffer = new char[1024];
	this->socket = socket;
}

void SSClient::clientClose()
{
	//close FD, free up any memory, return
	close(socket);
	free(buffer);
	return;
}

void SSClient::setName(std::string name)
{
	this->userName = name;
}

std::string SSClient::getName()
{
	return userName;
}

int SSClient::getSocket()
{
	return socket;
}

void SSClient::setSpreadsheet(SSnetworking* ss)
{
	this->spreadsheet = ss;
}

SSnetworking *SSClient::getSpreadsheet()
{
	return this->spreadsheet;
}

char* SSClient::readRequest()
{
	//change recv to read when go to linux
#ifdef __linux__
	int bytesRead = read(this->socket, this->buffer, sizeof(this->buffer));
#elif defined _WIN32
	int bytesRead = recv(this->socket, this->buffer, sizeof(this->buffer), 0);
#endif
	if (bytesRead == 0)
	{
		//remove client from clients
		close(this->socket);
		return NULL;
	}
	//null terminate the string so we dont use garbage information
	this->buffer[bytesRead] = 0;	
	return  this->buffer;
}

void SSClient::writeResponse(char* c)
{
	int bytesWrote = write(this->socket, c, strlen(c));
	return;

}
