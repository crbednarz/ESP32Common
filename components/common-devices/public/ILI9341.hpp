#pragma once
#include "gsl.hpp"
#include <array>
#include <driver/spi_master.h>


namespace esc {


class ILI9341
{
public:
	ILI9341(size_t width, size_t height);

	void connect();
	
	void writeCommand(uint8_t command);
	void writeData(uint8_t data);
	void writeData(gsl::span<const uint8_t> data);


private:
	bool _isConnected;
	size_t _width;
	size_t _height;
	std::array<spi_transaction_t, 7> _transactions;
	spi_device_handle_t _spiHandle;

	void setupPins();
	void setupSpi();

	enum class MessageMode
	{
		Data = 1,
		Command = 0
	};

	void write(gsl::span<const uint8_t> data, MessageMode mode);
};


}
