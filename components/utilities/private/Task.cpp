#include "Task.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void esc::Task::sleep(uint32_t milliseconds)
{
	vTaskDelay(milliseconds / portTICK_RATE_MS);
}
