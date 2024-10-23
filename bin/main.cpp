#include "libs/Config.h"
#include "libs/Weather.h"
#include "libs/interface.h"
#include <iostream>

void Run() {
    try {
        ConfigParameters config("C:/Users/igork/CLionProjects/labwork7-voldemar64/config.json");
        Weather weather(config);
        RunWeatherInterface(config, weather);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }
}

int main() {
    Run();
    return 0;
}