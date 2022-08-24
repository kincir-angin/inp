add_executable(in main.cpp App.cpp Utils.cpp)

include_directories(../include)
include_directories(../json/include)

target_link_libraries(in -larchive -lncurses nlohmann_json::nlohmann_json)