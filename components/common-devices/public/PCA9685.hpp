#pragma once
#include "I2CDevice.hpp"
#include <array>


class PCA9685
{
public:
	PCA9685();

	void connect();

	void setFrequency(float frequencyInHz);

	void set(uint8_t channel, float delta);

	void setRange(uint8_t channel, float minDutyInMs, float maxDutyInMs);

	void setDuty(uint8_t channel, uint16_t on, uint16_t off);

	void setDutyAll(uint16_t on, uint16_t off);

	bool isConnected() const;

	size_t channelCount() const;

private:
	class Channel
	{
	public:
		Channel();
		Channel(float minDutyInMs, float maxDutyInMs);

		float dutySize(float delta) const;
	
	private:
		float _minDutyInMs;
		float _maxDutyInMs;
	};

	I2CDevice _device;
	bool _isConnected;
	std::array<Channel, 12> _channels;
	float _dutyPerMs;
};
