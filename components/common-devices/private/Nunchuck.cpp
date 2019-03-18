#include "Nunchuck.hpp"
#include "Task.hpp"
#include <iostream>


Nunchuck::Nunchuck() :
	_device(0x52),
	_nextUpdate(xTaskGetTickCount())
{
	
}


void Nunchuck::connect()
{
	_device.write<uint8_t>(0xF0, 0x55);
	Task::sleep(500);
	_device.write<uint8_t>(0xFB, 0x00);
}


void Nunchuck::poll()
{
	constexpr TickType_t MIN_POLL_DELAY = static_cast<TickType_t>(0.02f * configTICK_RATE_HZ);;
	if (xTaskGetTickCount() < _nextUpdate)
		return;

	auto response = _device.read<std::array<uint8_t, 6>>();

	auto joystick = glm::vec2(response[0], response[1]) / 256.0f;
	_joystick = (joystick - 0.1f) / 0.8f;

	_nextUpdate = xTaskGetTickCount() + MIN_POLL_DELAY;
	_device.write<uint8_t>(0);
}


glm::vec2 Nunchuck::joystick() const
{
	return _joystick;
}
