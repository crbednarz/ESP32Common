#include "SSD1306.hpp"
#include "SSD1306Defines.hpp"



esc::SSD1306::SSD1306() :
	_device(0x3C)
{
	for (int i = 0; i < _buffer.size(); i += 2)
		_buffer[i] = 0xFF;
}


void esc::SSD1306::connect()
{
	static const uint8_t INIT_SEQUENCE[] = {
		DISPLAY_OFF,
		DISPLAY_SET_CONTRAST, 0xFF,
		DISPLAY_SET_ADDRESS_MODE, DISPLAY_ADDRESS_MODE_VERTICAL,
		DISPLAY_SET_COLUMN_ADDRESS, 0x00, 0x7F,
		DISPLAY_SET_PAGE_ADDRESS, 0x00, 0x07,
		DISPLAY_SET_START_LINE,
		DISPLAY_SET_SEGMENT_REMAP_ON,
		DISPLAY_SET_MULTIPLEX_RATIO, DISPLAY_MULTIPLEX_RESET,
		DISPLAY_SET_SCAN_DIRECTION_NORMAL,
		DISPLAY_SET_OFFSET, 0x00,
		DISPLAY_SET_CHARGE_PUMP, DISPLAY_CHARGE_PUMP_ON,
		DISPLAY_SET_COM_PINS, DISPLAY_COM_ALTERNATIVE,
		DISPLAY_SET_CLOCK_DIVIDE, 0xF0,
		DISPLAY_SET_PRECHARGE_PERIOD, 0x1,
		DISPLAY_SET_DESELECT_LEVEL, 0x20,
		DISPLAY_SET_NORMAL,
		DISPLAY_SHOW_RAM,
		DISPLAY_ON
	};
	
	for (int i = 0; i < 31; i++)
		writeCommand(INIT_SEQUENCE[i]);

	for (int x = 0; x < DISPLAY_WIDTH; x++)
		for (int y = 0; y < DISPLAY_COLUMN_PAGES; y++)
			writeData(0x00);
}


void esc::SSD1306::present(const Frame& frame)
{
    writeCommand(DISPLAY_SET_LOWER_COLUMN | (DISPLAY_WIDTH & 0xF));
    writeCommand(DISPLAY_SET_HIGHER_COLUMN | (DISPLAY_WIDTH >> 4));

	auto data = frame.data();
	for (int i = 0; i < data.size(); i++)
		_buffer[i * 2 + 1] = data[i];
	
	_device.write(gsl::span<const uint8_t>(_buffer.data(), _buffer.size()));
}


void esc::SSD1306::writeCommand(uint8_t command)
{
	const uint8_t message[] = { 0x80, command }; 
	_device.write(gsl::span<const uint8_t>(message, 2));
}


void esc::SSD1306::writeData(uint8_t data)
{
	const uint8_t message[] = { 0xFF, data }; 
	_device.write(gsl::span<const uint8_t>(message, 2));
}
