#include "Wifi.hpp"
#include "freertos/task.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include <iostream>
#include <cstring>


esc::Wifi::Wifi() :
	_eventGroup(xEventGroupCreate())
{
	wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
	tcpip_adapter_init();
	esp_event_loop_create_default();
	esp_wifi_init(&config);

	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &globalEventHandler, this);
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &globalEventHandler, this);
}


void esc::Wifi::connect(const std::string& ssid, const std::string& password)
{
	wifi_config_t config = {};
	strcpy(reinterpret_cast<char*>(config.sta.ssid), ssid.c_str());
	strcpy(reinterpret_cast<char*>(config.sta.password), password.c_str());

	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_set_config(ESP_IF_WIFI_STA, &config);
	esp_wifi_start();
	xEventGroupWaitBits(_eventGroup, BIT0, false, true, portMAX_DELAY);
}


void esc::Wifi::globalEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData)
{
	auto wifi = reinterpret_cast<Wifi*>(arg);
	wifi->eventHandler(eventBase, eventId, eventData);
}


void esc::Wifi::eventHandler(esp_event_base_t eventBase, int32_t eventId, void* eventData)
{
	if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED) {
		esp_wifi_connect();
		xEventGroupClearBits(_eventGroup, BIT0);
		std::cout << "Retrying connection to AP" << std::endl;
	} else if (eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP) {
		auto event = reinterpret_cast<ip_event_got_ip_t*>(eventData);
		std::cout << "Got IP: " << ip4addr_ntoa(&event->ip_info.ip) << std::endl;
		xEventGroupSetBits(_eventGroup, BIT0);
	}
}