#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../constant/constants.h"
#include "../common/commonType.h"
#include "../structure/dataType.h"
#include "../structure/hashMap.h"
#include "../structure/stringType.h"

#include "logger.h"

#define MAX_LOGGER_NUM 32

#define LOGGER_GATEWAY          "LOGGER_GATEWAY"

#define LOGGER_BALANCE          "LOGGER_BALANCE"

#define LOGGER_MENGINE          "LOGGER_MENGINE"

#define LOGGER_GATEWAY_PATH             "~/exchangesys/gateway/gateway.log"
#define LOGGER_GATEWAY_ERROR_PATH       "~/exchangesys/gateway/gateway_error.log"

#define LOGGER_BALANCE_PATH             "~/exchangesys/balance/balance.log"
#define LOGGER_BALANCE_ERROR_PATH       "~/exchangesys/balance/balance_error.log"

#define LOGGER_MENGINE_PATH             "~/exchangesys/mengine/mengine.log"
#define LOGGER_MENGINE_ERROR_PATH       "~/exchangesys/mengine/mengine_error.log"

HashMap *loggerMap = NULL;

static void info(Logger* logger, const char *formatter, ...);

static void warn(Logger* logger, const char *formatter, ...);

static void error(Logger* logger, const char *formatter, ...);

static void fatal(Logger* logger, const char *formatter, ...);

Logger *createLogger(char* normalFilepath, char* errorFilepath) {
    Logger *logger = (Logger *)malloc(sizeof(Logger));
    if (logger != NULL) {
        logger->info = info;
        logger->warn = warn;
        logger->error = error;
        logger->fatal = fatal;

        logger->normalFilepath = normalFilepath;
        logger->errorFilepath = errorFilepath;

        return logger;
    }
    return NULL;
}

void initLoggerFactory() {

    loggerMap = createHashMap(StringHashCode, StringEqualFun, MAX_LOGGER_NUM);

    String* gateWayLoggerKey = createString(LOGGER_GATEWAY);
    Logger* gateWayLogger = createLogger(LOGGER_GATEWAY_PATH, LOGGER_GATEWAY_ERROR_PATH);

    loggerMap->put(loggerMap, gateWayLoggerKey, gateWayLogger);

    String* balanceWayLoggerKey = createString(LOGGER_BALANCE);
    Logger* balanceWayLogger = createLogger(LOGGER_BALANCE_PATH, LOGGER_BALANCE_ERROR_PATH);

    loggerMap->put(loggerMap, balanceWayLoggerKey, balanceWayLogger);

    String* mengineWayLoggerKey = createString(LOGGER_MENGINE_PATH);
    Logger* mengineWayLogger = createLogger(LOGGER_MENGINE_PATH, LOGGER_MENGINE_ERROR_PATH);

    loggerMap->put(loggerMap, mengineWayLoggerKey, mengineWayLogger);

    printf("Logger component has been initialized successfully!\n");
}

Logger* getLogger(char* loggerName) {
    if (loggerMap == NULL) {
        printf("The logger factory has not been initialized Successfully, program will exit immediately!\n");
        exit(1);
    }
    String* loggerKey = createString(loggerName);
    if (loggerMap->containsKey(loggerMap, loggerKey)) {
        return loggerMap->get(loggerMap, loggerKey);
    }
    return NULL;
}

static void info(Logger* logger, const char *formatter, ...) {

}

static void warn(Logger* logger, const char *formatter, ...) {

}

static void error(Logger* logger, const char *formatter, ...) {

}

static void fatal(Logger* logger, const char *formatter, ...) {

}