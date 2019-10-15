#include "hmt_driver.h"

#include "wiringPi.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main() {
    wiringPiSetup();
    delay(1000);

    read_sensor();

    return 0;
}

#pragma clang diagnostic pop

