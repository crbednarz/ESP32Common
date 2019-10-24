#pragma once
#include "I2CDevice.hpp"
#include <array>


namespace esc {


class BME280
{
public:
	BME280();

	void connect();
	void poll();

	inline float temperature() const;
	inline float humidity() const;

private:
	I2CDevice _device;

	uint16_t _digT1;
	int16_t _digT2;
	int16_t _digT3;

	uint16_t _digP1;
	int16_t _digP2;
	int16_t _digP3;
	int16_t _digP4;
	int16_t _digP5;
	int16_t _digP6;
	int16_t _digP7;
	int16_t _digP8;
	int16_t _digP9;

	uint8_t _digH1;
	int16_t _digH2;
	int8_t _digH3;
	int16_t _digH4;
	int16_t _digH5;
	int8_t _digH6;

	float _temperature;
	float _humidity;

	bool isDeviceBusy();
	void recordTrimmings();

	int16_t readInt16(uint8_t header);
	uint16_t readUInt16(uint8_t header);
};


}


float esc::BME280::temperature() const
{
	return _temperature;
}


float esc::BME280::humidity() const
{
	return _humidity;
}