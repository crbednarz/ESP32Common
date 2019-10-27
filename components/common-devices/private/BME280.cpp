#include "BME280.hpp"
#include "Task.hpp"


esc::BME280::BME280() :
	_device(0x76),
	_temperature(0.0f),
	_humidity(0.0f)
{
}


void esc::BME280::connect()
{
	const uint8_t SOFT_RESET = 0xE0;
	const uint8_t CONTROL_REGISTER = 0xF4;
	const uint8_t HUMIDITY_CONTROL_REGISTER = 0xF2;
	const uint8_t CONFIG_REGISTER = 0xF5;

	_device.write<uint8_t>(SOFT_RESET, 0xB6);

	while (isDeviceBusy())
		Task::sleep(50.0f);

	recordTrimmings();

	// Sleep
	_device.write<uint8_t>(CONTROL_REGISTER, 0);

	_device.write<uint8_t>(HUMIDITY_CONTROL_REGISTER, 0b101);
	_device.write<uint8_t>(CONFIG_REGISTER, 0);

	// Wake-up
	_device.write<uint8_t>(CONTROL_REGISTER, 0b10010011);

	while (isDeviceBusy())
		Task::sleep(50.0f);
}


void esc::BME280::poll()
{
	const uint8_t TEMPERATURE_REGISTER = 0xFA;
	const uint8_t HUMIDITY_REGISTER = 0xFD;

	int32_t adc_T = 
		(_device.read<uint8_t>(TEMPERATURE_REGISTER + 0) << 16) |
		(_device.read<uint8_t>(TEMPERATURE_REGISTER + 1) <<  8) |
		(_device.read<uint8_t>(TEMPERATURE_REGISTER + 2) <<  0);
	adc_T >>= 4;

	int32_t adc_H = 
		(_device.read<uint8_t>(HUMIDITY_REGISTER + 0) << 8) |
		(_device.read<uint8_t>(HUMIDITY_REGISTER + 1) << 0);

	// Taken from 4.2.3 in datasheet:
	// https://cdn-shop.adafruit.com/datasheets/BST-BME280_DS001-10.pdf

	int32_t var1, var2, T, t_fine;

	var1 = ((((adc_T >> 3) - ((int32_t)_digT1 << 1))) * ((int32_t)_digT2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)_digT1)) * ((adc_T >> 4) - ((int32_t)_digT1))) >> 12) *
			((int32_t)_digT3)) >>
		   14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;

	_temperature = T / 100.0f;

	int32_t v_x1_u32r;
	v_x1_u32r = (t_fine - ((int32_t)76800));
	v_x1_u32r = (((((adc_H << 14) - (((int32_t)_digH4) << 20) - (((int32_t)_digH5) * v_x1_u32r)) +
				   ((int32_t)16384)) >>
				  15) *
				 (((((((v_x1_u32r * ((int32_t)_digH6)) >> 10) * (((v_x1_u32r *
																   ((int32_t)_digH3)) >>
																  11) +
																 ((int32_t)32768))) >>
					 10) +
					((int32_t)2097152)) *
					   ((int32_t)_digH2) +
				   8192) >>
				  14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)_digH1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	_humidity = (uint32_t)(v_x1_u32r >> 12) / 1024.0f;
}


bool esc::BME280::isDeviceBusy()
{
	const uint8_t ID_REGISTER = 0xD0;
	const uint8_t EXPECTED_ID = 0x60;
	auto id = _device.read<uint8_t>(ID_REGISTER);

	if (id != EXPECTED_ID)
		return true;

	constexpr uint8_t STATUS_REGISTER = 0xF3;
	auto status = _device.read<uint8_t>(STATUS_REGISTER);

	return (status & 0b1) == 0b1;
}


void esc::BME280::recordTrimmings()
{
	_digT1 = _device.read<uint16_t>(0x88);
	_digT2 = _device.read<int16_t>(0x8A);
	_digT3 = _device.read<int16_t>(0x8C);

	_digP1 = _device.read<uint16_t>(0x8E);
	_digP2 = _device.read<int16_t>(0x90);
	_digP3 = _device.read<int16_t>(0x92);
	_digP4 = _device.read<int16_t>(0x94);
	_digP5 = _device.read<int16_t>(0x96);
	_digP6 = _device.read<int16_t>(0x98);
	_digP7 = _device.read<int16_t>(0x9A);
	_digP8 = _device.read<int16_t>(0x9C);
	_digP9 = _device.read<int16_t>(0x9E);

	_digH1 = _device.read<uint8_t>(0xA1);
	_digH2 = _device.read<int16_t>(0xE1);
	_digH3 = _device.read<uint8_t>(0xE3);
	_digH4 = (_device.read<int8_t>(0xE4) << 4) |
			 (_device.read<int8_t>(0xE5) & 0xF);
	_digH5 = (_device.read<int8_t>(0xE6) << 4) |
			 (_device.read<int8_t>(0xE5) >> 4);
	_digH6 = _device.read<int8_t>(0xE7);
}


int16_t esc::BME280::readInt16(uint8_t header)
{
	auto value = _device.read<int16_t>(header);
	return ((value >> 8) & 0xFF) | (value << 8);
}


uint16_t esc::BME280::readUInt16(uint8_t header)
{
	auto value = _device.read<uint16_t>(header);
	return (value >> 8) | (value << 8);
}
