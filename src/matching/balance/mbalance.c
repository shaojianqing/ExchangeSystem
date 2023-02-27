#include <stdio.h>

#include "../../common/commonType.h"
#include "../../logger/logger.h"

int main(int argc, char **argv) {

    initLoggerFactory();

    Logger *logger = getLogger(LOGGER_BALANCE);

    logger->warn(logger, "This is the Test Logger Output, string:%s,integer:%d ", "Smith Shao", 111222333);

    printf("Matching Balance starts to run now!\n");
}