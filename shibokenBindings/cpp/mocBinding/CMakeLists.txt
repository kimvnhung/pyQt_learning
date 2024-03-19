# Required minimum cmake

cmake_minimum_required(VERSION 3.18)
cmake_policy(VERSION 3.18)

# Configure for Qt6
# Set CPP standard to C++17 minimum
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable policy to not use RPATH settings for install_name on macOS.
if(POLICY CMP0068)
    cmake_policy(SET CMP0068 NEW)
endif()

# Dynamic project name by folder name
get_filename_component(DYNAMIC_PR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${DYNAMIC_PR_NAME})

# For using lib in python
set(CONSUMER_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")

# Check for Qt installation
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent Multimedia MultimediaWidgets)

# find Shiboken
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/FindShiboken.cmake)

macro(add_qt_include_dirs QT_PACKAGES)
    foreach(QT_PACKAGE ${QT_PACKAGES})
        get_property(QT_INCLUDE_DIRS TARGET Qt6::${QT_PACKAGE} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

        foreach(INCLUDE_DIR ${QT_INCLUDE_DIRS})
            list(APPEND INCLUDES "-I${INCLUDE_DIR}")
        endforeach()
    endforeach()
endmacro()

set(INCLUDES "")
add_qt_include_dirs(Core Widgets Multimedia MultimediaWidgets Network Concurrent)