#include <stdio.h>

#include "../../common/commonType.h"
#include "../../logger/logger.h"

int main(int argc, char **argv) {

    initLoggerFactory();

    Logger *logger = getLogger(LOGGER_GATEWAY);

    logger->error(logger, "This is Error the Test Logger Output, string:%s,integer:%d ", "Smith Shao", 555666777);

    printf("Gateway starts to run now!\n");
}