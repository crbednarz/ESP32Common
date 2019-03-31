#include "RmtDriver.hpp"


esc::RmtDriver::RmtDriver(rmt_channel_t channel, gpio_num_t pin)
{
	rmt_tx_config_t txConfig = {};
	txConfig.loop_en = false;
	txConfig.carrier_freq_hz = 100;
	txConfig.carrier_duty_percent = 50;
	txConfig.carrier_level = RMT_CARRIER_LEVEL_LOW;
	txConfig.carrier_en = false;
	txConfig.idle_level = RMT_IDLE_LEVEL_LOW;
	txConfig.idle_output_en = true;

	rmt_config_t config = {};
	config.rmt_mode = RMT_MODE_TX;
	config.channel = channel;
	config.clk_div = 8;
	config.gpio_num = pin;
	config.mem_block_num = 1;
	config.tx_config = txConfig;

	assert(rmt_config(&config) == ESP_OK);
		
	assert(rmt_driver_install(config.channel, 0, 0) == ESP_OK);
	
	_handle.reset(channel);
}


void esc::RmtDriver::uninstall_driver(rmt_channel_t channel) 
{
	rmt_driver_uninstall(channel);
}