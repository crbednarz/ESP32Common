#include "InternetClock.hpp"
#include <time.h>
#include <sys/time.h>
#include "lwip/apps/sntp.h"



void esc::InternetClock::initialize()
{
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    
	time_t now = 0;
    struct tm timeinfo = {};
    int retry = 0;
    const int retry_count = 10;
    while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
}