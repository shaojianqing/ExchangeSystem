#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/epoll.h>

#include "../common/commonType.h"
#include "../constant/constants.h"
#include "../logger/logger.h"
#include "../network/network.h"
#include "../network/event.h"

#include "server.h"

static Logger *logger;

static bool start(HttpServer *this);

static bool initSocketAndEventloop(HttpServer *this);

static bool addHttpFilter(HttpServer *this, HttpFilter filter);

static bool setHttpProcessor(HttpServer *this, HttpProcessor processor);

static void LoggerFilter(HttpRequest *request, HttpResponse *response);

HttpServer* initHttpServer(u16 port) {

    logger = getLogger(LOGGER_HTTPSERVER);

    HttpServer *server = (HttpServer*)malloc(sizeof(HttpServer));
    server->start = start;
    server->addHttpFilter = addHttpFilter;
    server->setHttpProcessor = setHttpProcessor;
    server->addHttpFilter(server, LoggerFilter);
    server->serverPort = port;

    bool success = initSocketAndEventloop(server);
    if (success) {
        return server;
    } else {
        return NULL;
    }
}

static bool initSocketAndEventloop(HttpServer *this) {

    int serverSocketFd = prepareServerSocket(this->serverPort);
    if (serverSocketFd<0) {
        return false;
    }

    this->serverSocketFd = serverSocketFd;
    EventLoop *eventLoop = createEventLoop();
    if (eventLoop == NULL) {
        return false;
    }
}

static bool addHttpFilter(HttpServer *this, HttpFilter filter) {
    if (this->filterCount<MAX_FILTER_COUNT) {
        this->filters[this->filterCount] = filter;
        this->filterCount++;
        return true;
    } else {
        return false;
    }
}

static bool setHttpProcessor(HttpServer *this, HttpProcessor processor) {
    this->processor = processor;
    return true;
}

static bool start(HttpServer *this) {

    executeEventLoop(this->eventLoop);
    logger->info(logger, "HttpServer starts the service with the port:%d!", this->serverPort);
    return true;
}

static void LoggerFilter(HttpRequest *request, HttpResponse *response) {


}