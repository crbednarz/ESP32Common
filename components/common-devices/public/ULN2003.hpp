#pragma once
#include "GpioPin.hpp"
#include <array>


namespace esc {


class ULN2003
{
public:
	ULN2003(gpio_num_t pin1, gpio_num_t pin2, gpio_num_t pin3, gpio_num_t pin4);

	void step();

private:
	std::array<GpioPin, 4> _pins;
	uint8_t _state;

	void applyState();
};


}
