#include "GpioPin.hpp"


esc::GpioPin::GpioPin(gpio_num_t pin) :
	_pin(pin),
	_state(GpioPinState::Input)
{
	gpio_pad_select_gpio(_pin);
	gpio_set_direction(_pin, GPIO_MODE_INPUT);
}


void esc::GpioPin::setState(GpioPinState state)
{
	if (_state == state)
		return;
	
	if (state == GpioPinState::Input) {
		gpio_set_direction(_pin, GPIO_MODE_INPUT);
		_state = state;
		return;
	}

	if (_state == GpioPinState::Input) {
		gpio_set_direction(_pin, GPIO_MODE_OUTPUT);
	}

	_state = state;
	gpio_set_level(_pin, static_cast<uint32_t>(state));
}


bool esc::GpioPin::read() const
{
	assert(_state == GpioPinState::Input);

	return gpio_get_level(_pin) != 0;
}