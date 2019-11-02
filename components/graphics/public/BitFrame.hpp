#pragma once
#include <array>
#include <gsl.hpp>


namespace esc {


template <size_t Width, size_t Height>
class BitFrame
{
public:
	static_assert((Width * Height) % 8 == 0);

	BitFrame();

	constexpr size_t width() const;
	constexpr size_t height() const;

	gsl::span<const uint8_t> data() const; 

	uint8_t& segment(int x, int segmentY);	

	void state(int x, int y, bool state);

	void clear();

private:
	std::array<uint8_t, (Width * Height) / 8> _buffer;
};


}


template <size_t Width, size_t Height>
esc::BitFrame<Width, Height>::BitFrame()
{
	clear();
}


template <size_t Width, size_t Height>
constexpr size_t esc::BitFrame<Width, Height>::width() const
{
	return Width;
}


template <size_t Width, size_t Height>
constexpr size_t esc::BitFrame<Width, Height>::height() const
{
	return Height;
}


template <size_t Width, size_t Height>
gsl::span<const uint8_t> esc::BitFrame<Width, Height>::data() const
{
	return gsl::span<const uint8_t>(_buffer.data(), _buffer.size());
}



template <size_t Width, size_t Height>
uint8_t& esc::BitFrame<Width, Height>::segment(int x, int segmentY)
{
	return _buffer[(x * Height) / 8 + segmentY];
}


template <size_t Width, size_t Height>
void esc::BitFrame<Width, Height>::state(int x, int y, bool state)
{
	const uint8_t MASKS[] = {
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b10000000,
	};
	size_t index = (x * Height + y) / 8;
	size_t subIndex = (x * Height + y) % 8;
	if (state)
		_buffer[index] |= MASKS[subIndex];
	else
		_buffer[index] &= ~MASKS[subIndex];
}


template <size_t Width, size_t Height>
void esc::BitFrame<Width, Height>::clear()
{
	for (size_t i = 0; i < _buffer.size(); i++)
		_buffer[i] = 0;
}