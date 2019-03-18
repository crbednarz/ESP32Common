#pragma once


#include <stdint.h>


namespace esc {


class Task
{
public:
	Task() = delete;

	static void sleep(uint32_t milliseconds);
};


}
