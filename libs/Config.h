#pragma once

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include <fstream>
#include <string>
#include <vector>

class ConfigParameters {
public:
    int32_t number_of_days;
    uint64_t update_frequency;
    std::vector<std::string> cities;

    explicit ConfigParameters(const std::string& config_file_path);
};