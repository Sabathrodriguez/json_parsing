
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "SSClient.h"
#include "SSnetworking.h"

#define BUFFERSIZE 1024

SSClient::SSClient(int socket)
{
	this->buffer = new char[BUFFERSIZE];
	this->socket = socket;
	this->bufOffset = 0;
	this->bufSize = BUFFERSIZE;
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

	char * bufPtr = &(this->buffer[bufOffset]);
	//change recv to read when go to linux
	int bytesRead = read(this->socket, bufPtr, bufSize - bufOffset);
	if (bytesRead == 0)
	{
		//remove client from clients
		close(this->socket);
		return NULL;
	}
	bufOffset += bytesRead;
	//null terminate the string so we dont use garbage information
	this->buffer[bufOffset] = 0;	
	return  this->buffer;
}

void SSClient::resetBuffer()
{
	bufOffset = 0;
}

void SSClient::writeResponse(char* c)
{
	int bytesWrote = write(this->socket, c, strlen(c));
	return;

}
