#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdarg.h>

#include "../constant/constants.h"
#include "../common/commonType.h"
#include "../structure/dataType.h"
#include "../structure/hashMap.h"
#include "../structure/stringType.h"

#include "logger.h"

#define MAX_LOGGER_NUM                  32

#define MAX_MESSAGE_LENGTH              1024

#define LOGGER_GATEWAY_PATH             "/home/codespace/exchangesys/gateway/mgateway.log"
#define LOGGER_GATEWAY_ERROR_PATH       "/home/codespace/exchangesys/gateway/mgateway_error.log"

#define LOGGER_BALANCE_PATH             "/home/codespace/exchangesys/balance/mbalance.log"
#define LOGGER_BALANCE_ERROR_PATH       "/home/codespace/exchangesys/balance/mbalance_error.log"

#define LOGGER_MENGINE_PATH             "/home/codespace/exchangesys/mengine/mengine.log"
#define LOGGER_MENGINE_ERROR_PATH       "/home/codespace/exchangesys/mengine/mengine_error.log"

#define LOGGER_HTTPSERVER_PATH             "/home/codespace/exchangesys/httpserver/httpserver.log"
#define LOGGER_HTTPSERVER_ERROR_PATH       "/home/codespace/exchangesys/httpserver/httpserver_error.log"

HashMap *loggerMap = NULL;

static char messageBuffer[MAX_MESSAGE_LENGTH];

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

        logger->standardOutput = stdout;
        logger->normalLogFile = fopen(normalFilepath, "a+");
        logger->errorLogFile = fopen(errorFilepath, "a+");

        return logger;
    }
    return NULL;
}

void initLoggerFactory() {

    loggerMap = createHashMap(StringHashCode, StringEqualFun, MAX_LOGGER_NUM);

    Logger* gateWayLogger = createLogger(LOGGER_GATEWAY_PATH, LOGGER_GATEWAY_ERROR_PATH);
    loggerMap->put(loggerMap, LOGGER_GATEWAY, gateWayLogger);

    Logger* balanceLogger = createLogger(LOGGER_BALANCE_PATH, LOGGER_BALANCE_ERROR_PATH);
    loggerMap->put(loggerMap, LOGGER_BALANCE, balanceLogger);

    Logger* mengineLogger = createLogger(LOGGER_MENGINE_PATH, LOGGER_MENGINE_ERROR_PATH);
    loggerMap->put(loggerMap, LOGGER_MENGINE, mengineLogger);

    Logger* httpserverLogger = createLogger(LOGGER_HTTPSERVER_PATH, LOGGER_HTTPSERVER_ERROR_PATH);
    loggerMap->put(loggerMap, LOGGER_HTTPSERVER, httpserverLogger);

    printf("Logger component has been initialized successfully!\n");
}

Logger* getLogger(char* loggerName) {
    if (loggerMap == NULL) {
        printf("The logger factory has not been initialized Successfully, program will exit immediately!\n");
        exit(1);
    }
    if (loggerMap->containsKey(loggerMap, loggerName)) {
        return loggerMap->get(loggerMap, loggerName);
    }
    return NULL;
}

static void info(Logger* logger, const char *formatter, ...) {
    char *levelStr = NULL;
	char *errorStr = "";
	
    levelStr = "[INFO]";
	
	long now = time(NULL);
	char dateBuffer[32];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", localtime(&now));

    va_list argptr;
	va_start(argptr, formatter);

    int writeLength = vsnprintf(messageBuffer, MAX_MESSAGE_LENGTH, formatter, argptr);
	messageBuffer[writeLength] = '\0';
	
	va_end(argptr);

    fprintf(logger->standardOutput, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
    fflush(logger->standardOutput);

	fprintf(logger->normalLogFile, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
	fflush(logger->normalLogFile);
}

static void warn(Logger* logger, const char *formatter, ...) {

    char *levelStr = NULL;
	char *errorStr = "";
	
    levelStr = "[WARN]";
	
	long now = time(NULL);
	char dateBuffer[32];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
	
    va_list argptr;
	va_start(argptr, formatter);

    int writeLength = vsnprintf(messageBuffer, MAX_MESSAGE_LENGTH, formatter, argptr);
	messageBuffer[writeLength] = '\0';
	
	va_end(argptr);

    fprintf(logger->standardOutput, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
    fflush(logger->standardOutput);

    printf("logger->normalLogFile=>%s\n", logger->normalFilepath);

	fprintf(logger->normalLogFile, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
	fflush(logger->normalLogFile);
    
}

static void error(Logger* logger, const char *formatter, ...) {

    char *levelStr = NULL;
	char *errorStr = "";
	
    levelStr = "[ERROR]";
	
	long now = time(NULL);
	char dateBuffer[32];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
	
    va_list argptr;
	va_start(argptr, formatter);

    int writeLength = vsnprintf(messageBuffer, MAX_MESSAGE_LENGTH, formatter, argptr);
	messageBuffer[writeLength] = '\0';
	
	va_end(argptr);

    fprintf(logger->standardOutput, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
    fflush(logger->standardOutput);

	fprintf(logger->normalLogFile, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
	fflush(logger->normalLogFile);

    printf("logger->normalLogFile=>%s\n", logger->normalFilepath);

    fprintf(logger->errorLogFile, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
	fflush(logger->errorLogFile);
}

static void fatal(Logger* logger, const char *formatter, ...) {

    char *levelStr = NULL;
	char *errorStr = "";
	
    levelStr = "[FATAL]";
	
	long now = time(NULL);
	char dateBuffer[32];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    va_list argptr;
	va_start(argptr, formatter);

    int writeLength = vsnprintf(messageBuffer, MAX_MESSAGE_LENGTH, formatter, argptr);
	messageBuffer[writeLength] = '\0';
	
	va_end(argptr);
	
    fprintf(logger->standardOutput, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
    fflush(logger->standardOutput);

	fprintf(logger->normalLogFile, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
	fflush(logger->normalLogFile);

    fprintf(logger->errorLogFile, "%s %s %s %s\n", dateBuffer, levelStr, messageBuffer, errorStr);
	fflush(logger->errorLogFile);
}