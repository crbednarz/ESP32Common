#pragma once
#include "GpioPinState.hpp"
#include "Platform.hpp"


namespace esc {


class GpioPin
{
public:
	GpioPin(GpioNum pin);

	void setState(GpioPinState state);

	bool read() const;

private:
	GpioNum _pin;
	GpioPinState _state;

};


}
