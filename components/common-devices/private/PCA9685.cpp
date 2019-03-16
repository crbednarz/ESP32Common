#include "PCA9685.hpp"
#include "Task.hpp"
#include <cmath>


#define PCA9685_ADDRESS 0x40


PCA9685::PCA9685() :
	_device(PCA9685_ADDRESS),
	_isConnected(false),
	_dutyPerMs(0.0f)
{
	
}


void PCA9685::connect()
{
	if (isConnected())
		return;

	constexpr uint8_t MODE1 = 0x00;
	constexpr uint8_t MODE2 = 0x01;

	constexpr uint8_t SLEEP = 0x10;
	constexpr uint8_t ALLCALL = 0x01;
	constexpr uint8_t OUTDRV = 0x04;

	setDutyAll(0, 0);
	_device.write<uint8_t>(MODE2, OUTDRV);
	_device.write<uint8_t>(MODE1, ALLCALL);
	
	Task::sleep(5);

	auto mode = _device.read<uint8_t>(MODE1);
	mode &= ~SLEEP;
	_device.write<uint8_t>(MODE1, mode);

	Task::sleep(5);
	
	_isConnected = true;
}


void PCA9685::setFrequency(float frequencyInHz)
{
	constexpr uint8_t MODE1 = 0x00;
	constexpr uint8_t PRESCALE = 0xFE;

	float rawPrescale = round(25000000.0f / (4096.0f * frequencyInHz)) - 1.0f;
	int prescale = static_cast<int>(rawPrescale);

	auto oldMode = _device.read<uint8_t>(MODE1);
	uint8_t sleepMode = (oldMode & 0x7F) | 0x10;

	_device.write<uint8_t>(MODE1, sleepMode);
	_device.write<uint8_t>(PRESCALE, prescale);
	_device.write<uint8_t>(MODE1, oldMode);

	Task::sleep(5);

	_device.write<uint8_t>(MODE1, oldMode | 0x80);

	_dutyPerMs = (4096.0f * frequencyInHz) / 1000.0f;
}


void PCA9685::set(uint8_t channel, float delta)
{
	auto dutySizeInMs = _channels[channel].dutySize(delta);
	setDuty(channel, 0, static_cast<uint16_t>(dutySizeInMs * _dutyPerMs));
}


void PCA9685::setRange(uint8_t channel, float minDutyInMs, float maxDutyInMs)
{
	_channels[channel] = Channel(minDutyInMs, maxDutyInMs);
}
	

void PCA9685::setDuty(uint8_t channel, uint16_t on, uint16_t off)
{
	constexpr uint8_t LED0_ON_L = 0xFA;
	constexpr uint8_t LED0_ON_H = 0xFB;
	constexpr uint8_t LED0_OFF_L = 0xFC;
	constexpr uint8_t LED0_OFF_H = 0xFD;
	
	uint8_t offset = channel * 4;

	_device.write<uint8_t>(LED0_ON_L + offset, on & 0xFF);
	_device.write<uint8_t>(LED0_ON_H + offset, on >> 8);
	
	_device.write<uint8_t>(LED0_OFF_L + offset, off & 0xFF);
	_device.write<uint8_t>(LED0_OFF_H + offset, off >> 8);
}


void PCA9685::setDutyAll(uint16_t on, uint16_t off)
{
	constexpr uint8_t ALL_LED_ON_L = 0xFA;
	constexpr uint8_t ALL_LED_ON_H = 0xFB;
	constexpr uint8_t ALL_LED_OFF_L = 0xFC;
	constexpr uint8_t ALL_LED_OFF_H = 0xFD;
	
	_device.write<uint8_t>(ALL_LED_ON_L, on & 0xFF);
	_device.write<uint8_t>(ALL_LED_ON_H, on >> 8);
	
	_device.write<uint8_t>(ALL_LED_OFF_L, off & 0xFF);
	_device.write<uint8_t>(ALL_LED_OFF_H, off >> 8);
}


bool PCA9685::isConnected() const
{
	return _isConnected;
}


size_t PCA9685::channelCount() const
{
	return _channels.size();
}


PCA9685::Channel::Channel() :
	_minDutyInMs(1.0f),
	_maxDutyInMs(2.0f)
{
	
}


PCA9685::Channel::Channel(float minDutyInMs, float maxDutyInMs) :
	_minDutyInMs(minDutyInMs),
	_maxDutyInMs(maxDutyInMs)
{
	
}


float PCA9685::Channel::dutySize(float delta) const
{
	return (_maxDutyInMs - _minDutyInMs) * delta + _minDutyInMs;
}
