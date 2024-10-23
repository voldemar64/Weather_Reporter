include(FetchContent)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3020c34ae2b732121f37433e61599c34535e68a8)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_Declare(ftxui GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI GIT_TAG v4.0.0 )

FetchContent_MakeAvailable(cpr)
FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(ftxui)
FetchContent_GetProperties(ftxui)

add_library(Config Config.cpp Config.h)
add_library(Weather Weather.cpp Weather.h)
add_library(Interface Interface.cpp Interface.h Weather.cpp)

target_link_libraries(Config
        PUBLIC cpr::cpr
        PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(Weather
        PUBLIC cpr::cpr
        PUBLIC nlohmann_json::nlohmann_json
        PUBLIC Config)
target_link_libraries(Interface
        PUBLIC cpr::cpr
        PUBLIC nlohmann_json::nlohmann_json
        PUBLIC ftxui::dom
        PUBLIC ftxui::component
        PUBLIC ftxui::screen)