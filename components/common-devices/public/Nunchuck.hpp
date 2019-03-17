#pragma once


#include "I2CDevice.hpp"
#include "glm/vec2.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


class Nunchuck
{
public:
	Nunchuck();

	void connect();

	void poll();

	glm::vec2 joystick() const;

private:
	I2CDevice _device;

	glm::vec2 _joystick;
	TickType_t _nextUpdate;
};
