cmake_minimum_required(VERSION 3.18)

project(qt_av_player)

# Set CPP standard to C++17 minimum.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable policy to not use RPATH settings for install_name on macOS.
if(POLICY CMP0068)
    cmake_policy(SET CMP0068 NEW)
endif()

find_package(Qt6 REQUIRED
    COMPONENTS
    Core
    Widgets
)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# find source
file(GLOB HEADERS "${SOURCE_DIR}/*.h")
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Widgets
)