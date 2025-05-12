# filepath: /Users/fengjinwei/uol-cs/github/coursework/weather-graph/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(weather-graph)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add executable
set(SOURCES
    src/main.cpp
    src/WeatherMain.cpp
    src/models/Temp.cpp
    src/models/TempSet.cpp
    src/models/Candlestick.cpp
    src/utils/CSVReader.cpp
)

# Create the main executable
add_executable(weather-graph ${SOURCES})