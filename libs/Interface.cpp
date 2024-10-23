#include "interface.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <algorithm>
#include <iostream>
#include <conio.h>

using namespace ftxui;

void DisplayWeather(const std::string& city_name, const std::vector<Weather::DayWeather>& weatherData, int number_of_days) {
    std::vector<Element> forecast_days_elements;

    for (size_t i = 0; i < std::min(static_cast<int>(weatherData.size()), number_of_days); ++i) {
        const auto& daily = weatherData[i];
        std::vector<Element> day_periods;

        std::vector<std::string> times = {"00:00", "06:00", "12:00", "18:00"};

        for (const auto& time : times) {
            day_periods.push_back(
                    vbox({
                                 text(time) | border,
                                 text("Температура: " + std::to_string(daily.max_temperature) + "°C"),
                                 text("Мин. температура: " + std::to_string(daily.min_temperature) + "°C"),
                                 text("Скорость ветра: " + std::to_string(daily.max_wind_speed) + " m/s"),
                                 text("Осадки: " + std::to_string(daily.precipitation_sum) + " mm"),
                         }) | border
            );
        }

        forecast_days_elements.push_back(
                vbox({
                             text("Дата: " + daily.date) | bold | hcenter,
                             hbox(day_periods) | hcenter,
                     })
        );
    }

    Element document = vbox({
                                    text("Прогноз погоды для города: " + city_name) | bold | hcenter,
                                    vbox(forecast_days_elements),
                            }) | border;

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
}

void UpdateWeatherData(Weather& weather, const ConfigParameters& config, std::atomic<bool>& running) {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(config.update_frequency));
        Weather new_weather(config);
        weather.weather_in_cities = std::move(new_weather.weather_in_cities);
    }
}

void RunWeatherInterface(ConfigParameters& config, Weather& weather) {
    auto screen = ScreenInteractive::TerminalOutput();
    int current_city_index = 0;
    std::atomic<bool> running(true);

    std::thread update_thread(UpdateWeatherData, std::ref(weather), std::ref(config), std::ref(running));

    while (running) {
        auto& current_city = weather.weather_in_cities[current_city_index];
        DisplayWeather(current_city.name, current_city.days_weather, config.number_of_days);

        auto startTime = std::chrono::steady_clock::now();
        auto endTime = startTime + std::chrono::seconds(config.update_frequency);

        while (std::chrono::steady_clock::now() < endTime && running) {
            if (_kbhit()) {
                char key = _getch(); // Получаем символ без блокировки
                switch (key) {
                    case 27:
                        running = false;
                        break;
                    case 'n':
                        current_city_index = (current_city_index + 1) % weather.weather_in_cities.size();
                        break;
                    case 'p':
                        current_city_index = (current_city_index - 1 + weather.weather_in_cities.size()) % weather.weather_in_cities.size();
                        break;
                    case '+':
                        if (config.number_of_days < 10) {
                            config.number_of_days++;
                        }
                        break;
                    case '-':
                        if (config.number_of_days > 1) {
                            config.number_of_days--;
                        }
                        break;
                    default:
                        break; // Для любой другой клавиши ничего не делаем
                }
                break; // Переходим к следующему обновлению сразу после изменения настроек
            }
        }
    }

    running = false;
    if (update_thread.joinable()) {
        update_thread.join();
    }
}
