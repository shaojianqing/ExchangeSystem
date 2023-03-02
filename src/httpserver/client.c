#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <sys/epoll.h>

#include "../constant/constants.h"
#include "../common/commonType.h"
#include "../structure/stringType.h"

#include "../logger/logger.h"
#include "../network/network.h"
#include "../network/event.h"

#include "server.h"
#include "client.h"

#define REQUEST_BUFFER_SIZE 2048

extern Logger *logger;

static char requestBuffer[REQUEST_BUFFER_SIZE];

static void reply(HttpServer *server, HttpClient *client, String *info);

static void sendResponse(EventLoop *eventLoop, int fd, int mask, Object *object);

void acceptConnHandler(EventLoop *eventLoop, int fd, int mask, Object *object) {
	char ip[32];
	int port;
	int clientFd = acceptClientConn(fd, ip, &port);
	if (clientFd!=-1) {
		logger->info(logger, "Connecting client address:%s, port:%d\n", ip, port);
		HttpClient *client = createClient(clientFd);
		if (client!=NULL) {
			createNetworkEvent(eventLoop, clientFd, EVENT_READ_ABLE, processClientRequest, client);
		}
	}
}

HttpClient *createClient(int clientFd) {
    if (clientFd>0 && clientFd<EVENT_SIZE) {
		HttpClient *client  = malloc(sizeof(HttpClient));
		if (client!=NULL) {
			client->clientFd = clientFd;
			client->reply = reply;
			return client;
		}
	}
	return NULL;
}

void processClientRequest(EventLoop *eventLoop, int fd, int mask, Object *object) {
    if (eventLoop!=NULL && object!=NULL) {
		HttpClient *client = (HttpClient *)object;
		int done = 0;
		long count;
		char buf[512];

		memset(requestBuffer, 0, REQUEST_BUFFER_SIZE);
		count = read(fd, requestBuffer, sizeof(requestBuffer));
		if(count == -1) {
			if (errno != EAGAIN) {
				logger->error(logger, "Read from the client occur error!");
				done = 1;
			}
		} else if(count == 0) {
			done = 1;
		}

		String *requestString = createString(requestBuffer);

		printf("Received HTTP Request String:%s\n", requestString->value);

		//doProcessService(client);
	}
}

static void reply(HttpServer *server, HttpClient *client, String *info) {
    if (client!=NULL && info!=NULL) {
		client->responseBody = info;
		createNetworkEvent(server->eventLoop, client->clientFd, EVENT_WRITE_ABLE, sendResponse, client);
	}
}

static void sendResponse(EventLoop *eventLoop, int fd, int mask, Object *object) {
	if (eventLoop!=NULL && object!=NULL) {
		HttpClient *client = (HttpClient *)object;
		String *responseBody = client->responseBody;
		u64 writeBytes = write(fd, responseBody->value, responseBody->length);
		if (writeBytes == -1) {
			logger->error(logger, "Write to the client occur error!");
		} else {
			logger->error(logger, "Success send response to the client^+^");
		}
		deleteNetworkEvent(eventLoop, fd, mask);
	}
}