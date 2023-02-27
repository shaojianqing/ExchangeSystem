#include <stdio.h>

#include "../../common/commonType.h"
#include "../../logger/logger.h"


int main(int argc, char **argv) {

    initLoggerFactory();

    Logger *logger = getLogger(LOGGER_MENGINE);

    logger->info(logger, "This is the Test Logger Output, string:%s,integer:%d ", "Smith Shao", 888999666);

    printf("Matching Engine starts to run now!\n");
    
}