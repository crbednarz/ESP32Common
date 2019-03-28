#include "ULN2003.hpp"


esc::ULN2003::ULN2003(gpio_num_t pin1, gpio_num_t pin2, gpio_num_t pin3, gpio_num_t pin4) :
	_pins{GpioPin(pin1), GpioPin(pin2), GpioPin(pin3), GpioPin(pin4)},
	_state(0)
{
	applyState();
}


void esc::ULN2003::step()
{
	constexpr uint8_t MAX_STATES = 8;

	_state = (_state + 1) % MAX_STATES;
	applyState();
}


void esc::ULN2003::applyState()
{
	std::array<GpioPinState, 4> states;
	states.fill(GpioPinState::OutputLow);
	
	size_t primaryIndex = _state / 2; 
	size_t secondaryIndex = primaryIndex + (_state % 2);

	states[primaryIndex] = GpioPinState::OutputHigh;
	states[secondaryIndex] = GpioPinState::OutputHigh;

	for (int i = 0; i < states.size(); ++i)
		_pins[i].setState(states[i]);
}
