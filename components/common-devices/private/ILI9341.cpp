#include "ILI9341.hpp"
#include "ILI9341Defines.hpp"
#include <esp_system.h>
#include <driver/spi_master.h>
#include <soc/gpio_struct.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include "Task.hpp"


esc::ILI9341::ILI9341(size_t width, size_t height) :
	_isConnected(false),
	_width(width),
	_height(height)
{
	_transactions[0] = {};
	setupPins();
	setupSpi();
}



void esc::ILI9341::writeCommand(uint8_t command)
{
	gsl::span<uint8_t> message(&command, 1);
	write(message, MessageMode::Command);
}


void esc::ILI9341::writeData(uint8_t data)
{
	gsl::span<uint8_t> message(&data, 1);
	write(message, MessageMode::Data);
}


void esc::ILI9341::writeData(gsl::span<const uint8_t> data)
{
	write(data, MessageMode::Data);
}


void esc::ILI9341::connect()
{
	writeCommand(0x01);  // Software reset
 
	Task::sleep(5);
	
	writeCommand(0xEF);
	writeData(0x03);
	writeData(0x80);
	writeData(0x02);
 
	writeCommand(0xCF);
	writeData(0x00);
	writeData(0XC1);
	writeData(0X30);
 
	writeCommand(0xED);
	writeData(0x64);
	writeData(0x03);
	writeData(0X12);
	writeData(0X81);
 
	writeCommand(0xE8);
	writeData(0x85);
	writeData(0x00);
	writeData(0x78);
 
	writeCommand(0xCB);
	writeData(0x39);
	writeData(0x2C);
	writeData(0x00);
	writeData(0x34);
	writeData(0x02);
 
	writeCommand(0xF7);
	writeData(0x20);
 
	writeCommand(0xEA);
	writeData(0x00);
	writeData(0x00);
 
	writeCommand(ILI9341_PWCTR1);  //Power control
	writeData(0x23);               //VRH[5:0]
 
	writeCommand(ILI9341_PWCTR2);  //Power control
	writeData(0x10);               //SAP[2:0];BT[3:0]
 
	writeCommand(ILI9341_VMCTR1);  //VCM control
	writeData(0x3e);
	writeData(0x28);
 
	writeCommand(ILI9341_VMCTR2);  //VCM control2
	writeData(0x86);               //--
 
	writeCommand(ILI9341_MADCTL);  // Memory Access Control
	writeData(0x8);
 
	writeCommand(ILI9341_PIXFMT);
	writeData(0x55);
 
	writeCommand(ILI9341_FRMCTR1);
	writeData(0x00);
	writeData(0x13);  // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
 
	writeCommand(ILI9341_DFUNCTR);  // Display Function Control
	writeData(0x08);
	writeData(0x82);
	writeData(0x27);
 
	writeCommand(0xF2);  // 3Gamma Function Disable
	writeData(0x00);
 
	writeCommand(ILI9341_GAMMASET);  //Gamma curve selected
	writeData(0x01);
 
	writeCommand(ILI9341_GMCTRP1);  //Set Gamma
	writeData(0x0F);
	writeData(0x31);
	writeData(0x2B);
	writeData(0x0C);
	writeData(0x0E);
	writeData(0x08);
	writeData(0x4E);
	writeData(0xF1);
	writeData(0x37);
	writeData(0x07);
	writeData(0x10);
	writeData(0x03);
	writeData(0x0E);
	writeData(0x09);
	writeData(0x00);
 
	writeCommand(ILI9341_GMCTRN1);  //Set Gamma
	writeData(0x00);
	writeData(0x0E);
	writeData(0x14);
	writeData(0x03);
	writeData(0x11);
	writeData(0x07);
	writeData(0x31);
	writeData(0xC1);
	writeData(0x48);
	writeData(0x08);
	writeData(0x0F);
	writeData(0x0C);
	writeData(0x31);
	writeData(0x36);
	writeData(0x0F);
 
	writeCommand(ILI9341_SLPOUT);  //Exit Sleep
 
	Task::sleep(120);
 
	writeCommand(ILI9341_DISPON);  //Display on
	_isConnected = true;
}


void esc::ILI9341::present(gsl::span<const uint8_t> data)
{
	uint8_t caset[4] = { 
		0, 
		0, 
		static_cast<uint8_t>((_width >> 8) & 0xFF),
		static_cast<uint8_t>(_width & 0xFF)
	};
	uint8_t paset[4] = { 
		0, 
		0, 
		static_cast<uint8_t>((_height >> 8) & 0xFF),
		static_cast<uint8_t>(_height & 0xFF)
	};
	writeCommand(ILI9341_CASET);
	writeData(gsl::span<const uint8_t>(caset, 4));
	writeCommand(ILI9341_PASET);
	writeData(gsl::span<const uint8_t>(paset, 4));
	
	writeCommand(ILI9341_RAMWR);
	writeData(data);
}


void esc::ILI9341::setupPins()
{
	
	gpio_pad_select_gpio(DISPLAY_DC);
	gpio_pad_select_gpio(DISPLAY_RST);
	gpio_pad_select_gpio(DISPLAY_CS);

	//Initialize non-SPI GPIOs
	gpio_set_direction(DISPLAY_DC, GPIO_MODE_OUTPUT);
	gpio_set_direction(DISPLAY_RST, GPIO_MODE_OUTPUT);
	gpio_set_direction(DISPLAY_BCKL, GPIO_MODE_OUTPUT);
	gpio_set_direction(DISPLAY_CS, GPIO_MODE_OUTPUT);

	gpio_set_level(DISPLAY_CS, 1);
	
	//Reset the display
	gpio_set_level(DISPLAY_RST, 0);
	Task::sleep(100);
	gpio_set_level(DISPLAY_RST, 1);
	Task::sleep(100);
	gpio_set_level(DISPLAY_BCKL, 0);
	
	
	gpio_set_level(DISPLAY_CS, 0);
	
	ledc_timer_config_t ledcTimer = {};
	ledcTimer.freq_hz = 5000;
	ledcTimer.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledcTimer.timer_num = LEDC_TIMER_0;
	ledcTimer.duty_resolution = LEDC_TIMER_13_BIT;
	
	ledc_timer_config(&ledcTimer);
	
	ledc_channel_config_t ledcChannelConfig = {};
	ledcChannelConfig.channel = LEDC_CHANNEL_0;
	ledcChannelConfig.duty = 0;
	ledcChannelConfig.gpio_num = DISPLAY_BCKL;
	ledcChannelConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledcChannelConfig.timer_sel = LEDC_TIMER_0;
	
	ledc_channel_config(&ledcChannelConfig);
	
	ledc_fade_func_install(0);
	
	ledc_set_duty(ledcChannelConfig.speed_mode, ledcChannelConfig.channel, 0xFFF);
	ledc_update_duty(ledcChannelConfig.speed_mode, ledcChannelConfig.channel);
}


void esc::ILI9341::setupSpi()
{
	spi_bus_config_t spiBugConfig = {};
	spiBugConfig.mosi_io_num = DISPLAY_MOSI;
	spiBugConfig.miso_io_num = -1;
	spiBugConfig.sclk_io_num = DISPLAY_CLK;
	spiBugConfig.quadwp_io_num = -1;
	spiBugConfig.quadhd_io_num = -1;
	spiBugConfig.max_transfer_sz = _width * _height * sizeof(ILI9341Color) + 8;
	
	
	spi_device_interface_config_t deviceInterfaceConfig = {};
	deviceInterfaceConfig.clock_speed_hz = 40*1000*1000;
	deviceInterfaceConfig.mode = 0;
	deviceInterfaceConfig.spics_io_num = DISPLAY_CS;
	deviceInterfaceConfig.queue_size = 7;
	
	spi_bus_initialize(HSPI_HOST, &spiBugConfig, DISPLAY_DMA_CHANNEL);
	spi_bus_add_device(HSPI_HOST, &deviceInterfaceConfig, &_spiHandle);
}




void esc::ILI9341::write(gsl::span<const uint8_t> message, MessageMode mode)
{
	gpio_set_level(DISPLAY_DC, static_cast<uint32_t>(mode));	

	auto& transaction = _transactions[0];
	transaction.rx_buffer = nullptr;
	transaction.tx_buffer = message.data();
	transaction.length = message.size_bytes() * 8;
	transaction.rxlength = 0;
	transaction.flags = 0;
	spi_device_polling_transmit(_spiHandle, &transaction);
}