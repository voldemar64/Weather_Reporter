#pragma once

#include "Weather.h"
#include "Config.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <thread>
#include <atomic>
#include <chrono>

void RunWeatherInterface(ConfigParameters& config, Weather& weather);
void DisplayWeather(const std::string& city_name, const std::vector<Weather::DayWeather>& weatherData, int number_of_days);
