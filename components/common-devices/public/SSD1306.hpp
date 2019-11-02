#pragma once
#include "I2CDevice.hpp"
#include "BitFrame.hpp"


namespace esc {


class SSD1306
{
public:
	using Frame = BitFrame<128, 64>;

	SSD1306();

	void connect();

	void present(const Frame& frame);

private:
	I2CDevice _device;
	std::array<uint8_t, (128 * 64) / 4> _buffer;

	void writeCommand(uint8_t command);
	void writeData(uint8_t data);
};


}