#pragma once
#include "RmtDriver.hpp"
#include <array>
#include <stddef.h>
#include <glm/glm.hpp>
#include <driver/rmt.h>


namespace esc {


template <size_t Size>
class LedStrip 
{
public:
	LedStrip(rmt_channel_t channel, gpio_num_t pin);

	size_t size() const;

	void display();
	
	glm::u8vec3& operator[](size_t index);
	
private:
	RmtDriver _rmtDriver;
	std::array<rmt_item32_t, 24 * Size> _rmtItems;
	std::array<glm::u8vec3, Size> _colors;

	void writeColorToRmt(size_t index);
};


}


template <size_t Size>
esc::LedStrip<Size>::LedStrip(rmt_channel_t channel, gpio_num_t pin) :
	_rmtDriver(channel, pin)
{

}


template <size_t Size>
size_t esc::LedStrip<Size>::size() const
{
	return Size;
}

template <size_t Size>
void esc::LedStrip<Size>::display()
{
	rmt_wait_tx_done(_rmtDriver.channel(), 10000);

	for (int i = 0; i < Size; i++)
		writeColorToRmt(i);

	rmt_write_items(_rmtDriver.channel(), _rmtItems.data(), Size * 24, 0);
}


template <size_t Size>
glm::u8vec3& esc::LedStrip<Size>::operator[](size_t index)
{
	return _colors[index];
}


template <size_t Size>
void esc::LedStrip<Size>::writeColorToRmt(size_t index)
{
	rmt_item32_t rmtOn;
	rmtOn.level0 = 1;
	rmtOn.level1 = 0;
	rmtOn.duration0 = 9;
	rmtOn.duration1 = 3;

	rmt_item32_t rmtOff;
	rmtOff.level0 = 1;
	rmtOff.level1 = 0;
	rmtOff.duration0 = 3;
	rmtOff.duration1 = 9;

	auto color = _colors[index];
	int colorData = color.b;
	colorData = color.b | (color.r << 8) | (color.g << 16);
	for (int i = 0; i < 24; i++)
	{ 
		_rmtItems[i + index * 24] = (((colorData >> 23) & 1) == 1) ? rmtOn : rmtOff;
		colorData <<= 1;
	}
}
