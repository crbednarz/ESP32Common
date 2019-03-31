#pragma once
#include <driver/rmt.h>
#include "UniqueHandle.hpp"


namespace esc {


class RmtDriver
{
public:
	RmtDriver(rmt_channel_t channel, gpio_num_t pin);

	rmt_channel_t channel() const;

private:
	static void uninstall_driver(rmt_channel_t channel);
	
	using RmtHandle = UniqueHandle<rmt_channel_t, uninstall_driver, RMT_CHANNEL_MAX>;
	RmtHandle _handle;
};


}


inline rmt_channel_t esc::RmtDriver::channel() const
{
	return _handle.get();
}