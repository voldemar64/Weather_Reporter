#include "Config.h"

ConfigParameters::ConfigParameters(const std::string& config_file_path) {
    std::ifstream file(config_file_path, std::ios::binary);

    if (!file.is_open()) {
        throw std::invalid_argument("no json config file found");
    }
    auto config_parsed = nlohmann::json::parse(file);
    number_of_days = config_parsed["days"];
    cities = config_parsed["cities"];
    update_frequency = config_parsed["update-frequency"];
}