#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include <string>


namespace esc {


class Wifi
{
public:
	Wifi();

	Wifi(const Wifi&) = delete;
	Wifi(Wifi&&) = delete;
	Wifi& operator=(const Wifi&) = delete;
	Wifi& operator=(Wifi&&) = delete;

	void connect(const std::string& ssid, const std::string& password);

private:
	EventGroupHandle_t _eventGroup;
	wifi_init_config_t _config;

	static void globalEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);
	void eventHandler(esp_event_base_t eventBase, int32_t eventId, void* eventData);
};


}
