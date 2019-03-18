#pragma once


#include <stdint.h>
#include "gsl.hpp"


namespace esc {


class I2CDevice
{
public:
	static void initialize();

	I2CDevice(uint8_t slaveAddress);
	
	template <typename T>
	void write(T data) const;

	void write(gsl::span<const uint8_t> data) const;

	template <typename T>
	T read() const;

	void read(gsl::span<uint8_t> output) const;

	template <typename T>
	void write(uint8_t header, T data) const;

	void write(gsl::span<const uint8_t> header, gsl::span<const uint8_t> data) const;

	template <typename T>
	T read(uint8_t header) const;

	void read(gsl::span<const uint8_t> header, gsl::span<uint8_t> output) const;

private:
	uint8_t _slaveAddress;
	
};


}


template <typename T>
void esc::I2CDevice::write(T data) const
{
	write(gsl::span<const uint8_t>(reinterpret_cast<const uint8_t*>(&data), sizeof(T)));
}


template <typename T>
T esc::I2CDevice::read() const
{
	T data = T();
	read(gsl::span<uint8_t>(reinterpret_cast<uint8_t*>(&data), sizeof(T)));
	return data;
}


template <typename T>
void esc::I2CDevice::write(uint8_t header, T data) const
{
	write(gsl::span<const uint8_t>(&header, 1), 
		  gsl::span<const uint8_t>(reinterpret_cast<const uint8_t*>(&data), sizeof(T)));

}

template <typename T>
T esc::I2CDevice::read(uint8_t header) const
{
	T data = T();
	read(gsl::span<const uint8_t>(&header, 1), 
		  gsl::span<uint8_t>(reinterpret_cast<uint8_t*>(&data), sizeof(T)));
	return data;
}