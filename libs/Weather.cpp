#include "Weather.h"

Weather::Weather(const ConfigParameters& Params) {
    for (const auto& city_name : Params.cities) {
        Coordinates coordinates{};
        coordinates = getCoordinates(city_name);
        cpr::Response res = cpr::Get(
                cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                cpr::Parameters{{"latitude", std::to_string(coordinates.latitude)},
                                {"longitude", std::to_string(coordinates.longitude)},
                                {"timezone", "auto"},
                                {"forecast_days", std::to_string(Params.number_of_days)},
                                {"daily", "temperature_2m_max"},
                                {"daily", "temperature_2m_min"},
                                {"daily", "windspeed_10m_max"},
                                {"daily", "weathercode"},
                                {"daily", "precipitation_sum"}});

        if (res.status_code != 200) {
            throw std::runtime_error("Error: " + std::to_string(res.status_code));
        }

        auto response_parsed = nlohmann::json::parse(res.text);
        CityWeather current_city;
        current_city.name = city_name;
        for (size_t i = 0; i < Params.number_of_days; ++i) {
            DayWeather Day{};
            Day.date = response_parsed["daily"]["time"].at(i).get<std::string>();
            Day.WMO_code = response_parsed["daily"]["weathercode"].at(i).get<uint8_t>();
            Day.weather_description = getWeatherDescription(Day.WMO_code);
            Day.max_temperature = response_parsed["daily"]["temperature_2m_max"].at(i).is_null() ? 0
                                                                                                 : response_parsed["daily"]["temperature_2m_max"].at(
                            i).get<int8_t>();
            Day.min_temperature = response_parsed["daily"]["temperature_2m_min"].at(i).is_null() ? 0
                                                                                                 : response_parsed["daily"]["temperature_2m_min"].at(
                            i).get<int8_t>();
            Day.max_wind_speed = response_parsed["daily"]["windspeed_10m_max"].at(i).is_null() ? 0
                                                                                               : response_parsed["daily"]["windspeed_10m_max"].at(
                            i).get<int8_t>();
            Day.precipitation_sum = response_parsed["daily"]["precipitation_sum"].at(i).is_null() ? 0.0
                                                                                                  : response_parsed["daily"]["precipitation_sum"].at(
                            i).get<double>();
            current_city.days_weather.push_back(Day);
        }

        weather_in_cities.push_back(current_city);
    }
}

Weather::Coordinates Weather::getCoordinates(const std::string& city_name) {
    std::string token = "Rp4iovzhDMhChOSlUHsdBg==oFhy7V3ZC3LDsiIQ";
    cpr::Response response = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                                      cpr::Header{{"X-Api-Key", token}},
                                      cpr::Parameters{{"name", city_name}});
    if (response.status_code != 200) {
        throw std::runtime_error("Request to api-ninjas failed");
    }
    auto response_parsed = nlohmann::json::parse(response.text);
    return {response_parsed[0]["latitude"], response_parsed[0]["longitude"]};
}

std::string Weather::getWeatherDescription(uint8_t WMO_code) {
    switch (WMO_code) {
        case 0: return "Clear sky";
        case 1: return "Mainly clear";
        case 2: return "Partly cloudy";
        case 3: return "Overcast";
        case 45: return "Fog";
        case 48: return "Depositing rime fog";
        case 51: return "Light drizzle";
        case 53: return "Moderate drizzle";
        case 55: return "Dense drizzle";
        case 56: return "Light freezing drizzle";
        case 57: return "Dense freezing drizzle";
        case 61: return "Slight rain";
        case 63: return "Moderate rain";
        case 65: return "Heavy rain";
        case 66: return "Light freezing rain";
        case 67: return "Heavy freezing rain";
        case 71: return "Light snow fall";
        case 73: return "Moderate snow fall";
        case 75: return "Heavy snow fall";
        case 77: return "Snow grains";
        case 80: return "Slight rain showers";
        case 81: return "Moderate rain showers";
        case 82: return "Violent rain showers";
        case 85: return "Slight snow showers";
        case 86: return "Heavy snow showers";
        case 95: return "Thunderstorm";
        case 96: return "Thunderstorm with slight hail";
        case 99: return "Thunderstorm with heavy hail";
        default: return "Aliens attack";
    }
}