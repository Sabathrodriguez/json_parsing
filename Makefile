
CC = g++
OBJS = SSserver.o SSClient.o SSnetworking.o       
CXXFLAGS = -g 
LIBS = 

Spreadsheet: $(OBJS) 
	$(CC) -o server $(CXXFLAGS) $(OBJS) $(LIBS)

	
SSClient.o: SSClient.cpp SSClient.h

SSserver.o: SSserver.cpp SSserver.h

SSnetworking.o: SSnetworking.cpp SSnetworking.h


clean:
	rm $(OBJS) Spreadsheet
