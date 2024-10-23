#pragma once

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "Config.h"
#include <fstream>
#include <string>
#include <vector>

class Weather {
public:
    struct DayWeather {
        std::string date;
        std::string weather_description;
        uint8_t WMO_code;
        int8_t max_temperature;
        int8_t min_temperature;
        int8_t max_wind_speed;
        double precipitation_sum;
    };

    struct CityWeather {
        std::string name;
        std::vector<DayWeather> days_weather;
    };

    std::vector<CityWeather> weather_in_cities;

    Weather() = default;
    explicit Weather(const ConfigParameters& Params);
private:
    struct Coordinates {
        double latitude;
        double longitude;

        Coordinates() = default;
        Coordinates(double latitude, double longitude)
                : latitude(latitude), longitude(longitude) {}
    };

    static Coordinates getCoordinates(const std::string& city_name);
    static std::string getWeatherDescription(uint8_t WMO_code);
};
