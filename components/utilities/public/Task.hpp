#pragma once


#include <stdint.h>


class Task
{
public:
	Task() = delete;

	static void sleep(uint32_t milliseconds);
};
