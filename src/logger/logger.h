
typedef struct Logger Logger;

#define LEVEL_INOF              1
#define LEVEL_WARN              2
#define LEVEL_ERROR             3
#define LEVEL_FATAL             4

#define LOGGER_GATEWAY          "LOGGER_GATEWAY"
#define LOGGER_BALANCE          "LOGGER_BALANCE"
#define LOGGER_MENGINE          "LOGGER_MENGINE"

#define LOGGER_HTTPSERVER       "LOGGER_HTTPSERVER"

struct Logger {

    char *normalFilepath;

    char *errorFilepath;

    FILE *normalLogFile;

    FILE *errorLogFile;

    FILE *standardOutput;

    void (*info)(Logger* logger, const char *formatter, ...);

    void (*warn)(Logger* logger, const char *formatter, ...);

    void (*error)(Logger* logger, const char *formatter, ...);

    void (*fatal)(Logger* logger, const char *formatter, ...);
};

void initLoggerFactory();

Logger* createLogger(char* normalFilepath, char* errorFilepath);

Logger* getLogger(char* loggerName);