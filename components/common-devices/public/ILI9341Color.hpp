#pragma once


#include <stdint.h>


namespace esc {


class ILI9341Color
{
public:
	using RawType = uint16_t;

	constexpr ILI9341Color();
	constexpr ILI9341Color(uint8_t r, uint8_t g, uint8_t b);

	constexpr RawType raw() const;

	constexpr bool operator==(ILI9341Color rhs) const;
	constexpr bool operator!=(ILI9341Color rhs) const;

	constexpr static ILI9341Color fromBrightness(uint8_t brightness);

private:
	RawType _value;
};


}


constexpr esc::ILI9341Color::ILI9341Color() :
	_value(0)
{
	
}


constexpr esc::ILI9341Color::ILI9341Color(uint8_t r, uint8_t g, uint8_t b) :
	_value(
		(r & 0b11111000) |		// xxxxxxxxRRRRRxxx
		((b >> 3) << 8) |		// xxxBBBBBxxxxxxxx
		(g >> 5) |				// xxxxxxxxxxxxxGGG
		((g & 0b11100) << 11))	// GGGxxxxxxxxxxxxx
{

}


constexpr esc::ILI9341Color::RawType esc::ILI9341Color::raw() const
{
	return _value;
}


constexpr bool esc::ILI9341Color::operator==(ILI9341Color rhs) const
{
	return _value == rhs._value;
}


constexpr bool esc::ILI9341Color::operator!=(ILI9341Color rhs) const
{
	return _value != rhs._value;
}


constexpr esc::ILI9341Color esc::ILI9341Color::fromBrightness(uint8_t brightness)
{
	return ILI9341Color(brightness, brightness, brightness);
}