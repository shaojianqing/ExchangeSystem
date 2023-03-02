#include <stdio.h>

#include "../../common/commonType.h"
#include "../../logger/logger.h"
#include "../../httpserver/server.h"

static Logger *logger;

static void httpProcessor(HttpRequest *request, HttpResponse *response);

int main(int argc, char **argv) {

    initLoggerFactory();

    logger = getLogger(LOGGER_GATEWAY);

    logger->info(logger, "MGateway server starts to run now!");

    HttpServer *server = initHttpServer(8888);
    server->setHttpProcessor(server, httpProcessor);
    server->start(server);
}

static void httpProcessor(HttpRequest *request, HttpResponse *response) {

}