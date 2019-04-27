#pragma once
#include <array>
#include <memory>
#include "gsl.hpp"


namespace esc {


template <typename TColor, size_t Width, size_t Height>
class Frame {
public:
	Frame() = default;

	size_t width() const;
	size_t height() const;

	TColor& color(size_t x, size_t y);
	const TColor& color(size_t x, size_t y) const;

	gsl::span<const uint8_t> rawData() const;
	gsl::span<const uint8_t> rawRow(int y) const;

private:
	std::array<TColor, Width * Height> _buffer;

};


}


template <typename TColor, size_t Width, size_t Height>
size_t esc::Frame<TColor, Width, Height>::width() const
{
	return Width;
}


template <typename TColor, size_t Width, size_t Height>
size_t esc::Frame<TColor, Width, Height>::height() const
{
	return Height;
}


template <typename TColor, size_t Width, size_t Height>
TColor& esc::Frame<TColor, Width, Height>::color(size_t x, size_t y)
{
	return _buffer[y * Width + x];
}


template <typename TColor, size_t Width, size_t Height>
const TColor& esc::Frame<TColor, Width, Height>::color(size_t x, size_t y) const
{
	return _buffer[y * Width + x];
}


template <typename TColor, size_t Width, size_t Height>
gsl::span<const uint8_t> esc::Frame<TColor, Width, Height>::rawData() const
{
	return gsl::span<const uint8_t>(
		reinterpret_cast<const uint8_t*>(_buffer.data()),
		_buffer.size() * sizeof(TColor)
	);
}


template <typename TColor, size_t Width, size_t Height>
gsl::span<const uint8_t> esc::Frame<TColor, Width, Height>::rawRow(int y) const
{
	const uint8_t* rowData = reinterpret_cast<const uint8_t*>(&_buffer[Width * y]);
	return gsl::span<const uint8_t>(rowData, Height * sizeof(TColor));
}
