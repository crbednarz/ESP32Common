#include "Task.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <chrono>


void esc::Task::sleep(uint32_t milliseconds)
{
	vTaskDelay(milliseconds / portTICK_RATE_MS);
}


void esc::Task::wait(float milliseconds)
{
	using Clock = std::chrono::high_resolution_clock;

	auto currentTime = Clock::now();
	auto duration = static_cast<int64_t>(milliseconds * 1000000.0f);
	auto targetTime = currentTime + std::chrono::nanoseconds(duration);
	while (Clock::now() < targetTime) {
		// Wait
	}
 }