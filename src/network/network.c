#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <errno.h>

#include "../common/commonType.h"
#include "../constant/constants.h"

#include "../structure/stringType.h"
#include "../structure/dataType.h"
#include "../structure/hashMap.h"

#include "event.h"
#include "network.h"

int serverSocketFd;

int clientSocketFd;

int addressLength;

EpollEvent serverEvent;

EpollEvent clientEvent;

EpollEvent targetEvent[MAX_CONNECT];

int prepareServerSocket(u16 port) {

	SocketInetAddress serverAddress, clientAddress;

	int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd==-1) {
		//printLog(LEVEL_ERROR, "Create Socket Error!");
		return -1;
	} else {
		//printLog(LEVEL_INFO, "Create Socket Successfully^_^");	
	}
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);

	int status = bind(serverSocketFd, (SocketAddress *)&serverAddress, sizeof(SocketAddress));
	if (status==-1) {
		//printLog(LEVEL_ERROR, "Bind Socket Error!");
		return -1;	
	} else {
		//printLog(LEVEL_INFO, "Bind Socket Successfully^_^");	
	}

	status = listen(serverSocketFd, 20);
	if (status==-1) {
		//printLog(LEVEL_ERROR, "Bind Socket Error:");
		return -1;	
	} else {
		//printLog(LEVEL_INFO, "Start to Listen Successfully^_^");
	}
	return serverSocketFd;
}

int acceptClientConn(int serverFd, char *ip, int *port) {
	int clientFd = 0;
	SocketInetAddress address;
	u32 addressLength = sizeof(SocketInetAddress);
	while(true) {
		clientFd = accept(serverFd, (SocketAddress *)&address, &addressLength);
		if (clientFd==-1) {
			if (errno==EINTR) {
				continue;			
			} else {
				//printLog(LEVEL_ERROR, "Accept Connection Error:");
				return INET_ERROR;
			}
		}
		break;
	}
	if (ip!=NULL) {
		strcpy(ip, inet_ntoa(address.sin_addr));	
	}
	if (port!=NULL) {
		*port = ntohs(address.sin_port);	
	}
	return clientFd;
}