#pragma once
#include "GpioPinState.hpp"
#include <driver/gpio.h>


namespace esc {


class GpioPin
{
public:
	GpioPin(gpio_num_t pin);

	void setState(GpioPinState state);

	bool read() const;

private:
	gpio_num_t _pin;
	GpioPinState _state;

};


}
