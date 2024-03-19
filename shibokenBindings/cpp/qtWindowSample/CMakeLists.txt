# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.18)
cmake_policy(VERSION 3.18)

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

# Get the name of the folder containing the CMakeLists.txt file
get_filename_component(DYNAMIC_PR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)


# Consider changing the project name to something relevant for you.
project(${DYNAMIC_PR_NAME})

# ================================ General configuration ======================================
set(CONSUMER_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent Multimedia MultimediaWidgets Network Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent Multimedia MultimediaWidgets Network Gui)

# ================================== Shiboken detection ======================================
# Use provided python interpreter if given.
if(NOT python_interpreter)
    if(WIN32 AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        find_program(python_interpreter "python_d")

        if(NOT python_interpreter)
            message(FATAL_ERROR
                "A debug Python interpreter could not be found, which is a requirement when "
                "building this example in a debug configuration. Make sure python_d.exe is in "
                "PATH.")
        endif()
    else()
        find_program(python_interpreter "python")

        if(NOT python_interpreter)
            message(FATAL_ERROR
                "No Python interpreter could be found. Make sure python is in PATH.")
        endif()
    endif()
endif()


message(STATUS "Using python interpreter: ${python_interpreter}")

# Add pyside config
set(PYSIDE_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/../../../utils/pyside_config.py")

if(NOT EXISTS ${PYSIDE_CONFIG})
    message(FATAL_ERROR "No such file ${PYSIDE_CONFIG}")
endif()

# Macro to get various pyside / python include / link flags and paths.
# Uses the not entirely supported utils/pyside_config.py file.
macro(pyside_config option output_var)
    if(${ARGC} GREATER 2)
        set(is_list ${ARGV2})
    else()
        set(is_list "")
    endif()

    execute_process(
        COMMAND ${python_interpreter} "${PYSIDE_CONFIG}"
        ${option}
        OUTPUT_VARIABLE ${output_var}
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    if("${${output_var}}" STREQUAL "")
        message(FATAL_ERROR "Error: Calling pyside_config.py ${option} returned no output.")
    endif()

    if(is_list)
        string(REPLACE " " ";" ${output_var} "${${output_var}}")
    endif()
endmacro()

# Query for the shiboken generator path, Python path, include paths and linker flags.
pyside_config(--shiboken-module-path SHIBOKEN_MODULE_PATH)
pyside_config(--shiboken-generator-path SHIBOKEN_GENERATOR_PATH)
pyside_config(--pyside-path PYSIDE_PATH)

pyside_config(--python-include-path PYTHON_INCLUDE_DIR)
pyside_config(--shiboken-generator-include-path SHIBOKEN_GENERATOR_INCLUDE_DIR 1)
pyside_config(--pyside-include-path PYSIDE_INCLUDE_DIR 1)

pyside_config(--python-link-flags-cmake PYTHON_LINKING_DATA 0)
pyside_config(--shiboken-module-shared-libraries-cmake SHIBOKEN_MODULE_SHARED_LIBRARIES 0)
pyside_config(--pyside-shared-libraries-cmake PYSIDE_SHARED_LIBRARIES 0)

set(SHIBOKEN_PATH "${SHIBOKEN_GENERATOR_PATH}/shiboken6${CMAKE_EXECUTABLE_SUFFIX}")

if(NOT EXISTS ${SHIBOKEN_PATH})
    message(FATAL_ERROR "Shiboken executable not found at path: ${SHIBOKEN_PATH}")
endif()

macro(add_qt_include_dirs)
    foreach(QT_PACKAGE ${ARGN})
        get_property(QT_INCLUDE_DIRS TARGET Qt6::${QT_PACKAGE} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

        foreach(INCLUDE_DIR ${QT_INCLUDE_DIRS})
            list(APPEND INCLUDES "-I${INCLUDE_DIR}")
        endforeach()
    endforeach()
endmacro()

set(INCLUDES "")
add_qt_include_dirs(Core Widgets Multimedia MultimediaWidgets Network Concurrent Gui)
message("INCLUDE ${INCLUDES}")


# On macOS, check if Qt is a framework build. This affects how include paths should be handled.
get_target_property(QtCore_is_framework Qt6::Core FRAMEWORK)
if (QtCore_is_framework)
    get_target_property(qt_core_library_location Qt6::Core LOCATION)
    # PYSIDE-623: We move up until the directory contains all the frameworks.
    #             This is "lib" in ".../lib/QtCore.framework/Versions/A/QtCore".
    get_filename_component(lib_dir "${qt_core_library_location}/../../../.." ABSOLUTE)
    list(APPEND INCLUDES "--framework-include-paths=${lib_dir}")
endif()

# =============================== CMake target - sample_library ===============================

# The sample library for which we will create bindings. You can change the name to something
# relevant for your project.
set(LIBRARY_NAME "lib${PROJECT_NAME}")

# The name of the generated bindings module (as imported in Python). You can change the name
# to something relevant for your project.
set(BINDING_NAME "${PROJECT_NAME}")

configure_file(bindings.xml.in bindings.xml)

# Set the CPP files.
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${SOURCE_DIR}/*.h)
list(APPEND SOURCES ${GENERATED_SOURCES})

# Set up the options to pass to shiboken.
set(WRAPPED_HEADER ${SOURCE_DIR}/bindings.h)
set(TYPESYSTEM_FILE ${CMAKE_CURRENT_BINARY_DIR}/bindings.xml)

message("includes " ${INCLUDES})
list(APPEND INCLUDES "-I${CMAKE_CURRENT_BINARY_DIR}")

set(SHIBOKEN_OPTIONS
    --generator-set=shiboken
    --enable-parent-ctor-heuristic
    --enable-pyside-extensions
    --enable-return-value-heuristic
    --use-isnull-as-nb_nonzero
    --avoid-protected-hack
    ${INCLUDES}
    -I${CMAKE_SOURCE_DIR}
    -T${CMAKE_SOURCE_DIR}
    -T${PYSIDE_PATH}/typesystems
    --output-directory=${CMAKE_CURRENT_BINARY_DIR}
)

# Specify which sources will be generated by shiboken, and their dependencies.
set(GENERATED_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/${BINDING_NAME}/${PROJECT_NAME}_module_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${BINDING_NAME}/customwidget_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${BINDING_NAME}/myplayer_wrapper.cpp
    # ${CMAKE_CURRENT_BINARY_DIR}/${BINDING_NAME}/avplayermanager_wrapper.cpp
    # ${CMAKE_CURRENT_BINARY_DIR}/${BINDING_NAME}/qavvideoframe_wrapper.cpp
)

set(GENERATED_SOURCES_DEPENDENCIES
    ${WRAPPED_HEADER}
    ${TYPESYSTEM_FILE}
)

# Add custom target to run shiboken.
add_custom_command(OUTPUT ${GENERATED_SOURCES}
    COMMAND ${SHIBOKEN_PATH}
    ${SHIBOKEN_OPTIONS} ${WRAPPED_HEADER} ${TYPESYSTEM_FILE}
    DEPENDS ${GENERATED_SOURCES_DEPENDENCIES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running generator for ${TYPESYSTEM_FILE}.")



# We need to include the headers for the module bindings that we use.
set(PYSIDE_ADDITIONAL_INCLUDES "")

foreach(INCLUDE_DIR ${PYSIDE_INCLUDE_DIR})
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtCore")
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtGui")
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtMultimedia")
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtMultimediaWidgets")
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtWidgets")
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtNetwork")
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtConcurrent")
endforeach()

# =============================================================================================
# !!! (The section below is deployment related, so in a real world application you will want to
# take care of this properly with some custom script or tool).
# =============================================================================================
# Enable rpaths so that the example can be executed from the build dir.
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH ${PYSIDE_PATH} ${SHIBOKEN_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# =============================================================================================
# !!! End of dubious section.
# =============================================================================================





# Add the rest of the sources.
add_library(${LIBRARY_NAME} SHARED ${SOURCES} ${HEADERS})
target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::Network
)
target_include_directories(${LIBRARY_NAME}
    PUBLIC
        ${CMAKE_BINARY_DIR}
)

# ====================================== Custom Library =======================================
set(ENABLE_MULTIMEDIA "TRUE")
add_subdirectory(QtAVPlayer)
target_link_libraries(${LIBRARY_NAME} PRIVATE QtAVPlayer)
# copy shared dependencies
file(GLOB ${LIBRARY_NAME}_DEPENDENCIES "${CMAKE_BINARY_DIR}/QtAVPlayer/*.dll")

# find_package(QtAVPlayer REQUIRED)

# ====================================== End Config Custom Lib ================================


set_property(TARGET ${LIBRARY_NAME} PROPERTY PREFIX "")

# Needed mostly on Windows to export symbols, and create a .lib file, otherwise the binding
# library can't link to the sample library.
target_compile_definitions(${LIBRARY_NAME} PRIVATE BINDINGS_BUILD)
target_include_directories(${LIBRARY_NAME} PRIVATE ${INCLUDES})
target_include_directories(${LIBRARY_NAME} PUBLIC ${HEADERS})

# ==================================== For testing ====================================
# set(TEST_PROGRAM ${PROJECT_NAME}_test)
# add_executable(${TEST_PROGRAM} main.cpp)
# target_link_libraries(${TEST_PROGRAM}
#     PRIVATE
#         Qt6::Core
#         Qt6::Gui
#         Qt6::Widgets
#         Qt6::Multimedia
#         Qt6::MultimediaWidgets
#         Qt6::Network
#         ${LIBRARY_NAME}
# )

# target_include_directories(${TEST_PROGRAM}
#     PRIVATE
#         ${HEADERS}
# )


# =======================================================================================


# Set the cpp files which will be used for the bindings library.
set(${BINDING_NAME}_SOURCES ${GENERATED_SOURCES})

# Define and build the bindings library.
add_library(${BINDING_NAME} MODULE ${${BINDING_NAME}_SOURCES})

# Apply relevant include and link flags.
target_include_directories(${BINDING_NAME} PRIVATE ${PYTHON_INCLUDE_DIR})
target_include_directories(${BINDING_NAME} PRIVATE ${SHIBOKEN_GENERATOR_INCLUDE_DIR})
target_include_directories(${BINDING_NAME} PRIVATE ${PYSIDE_INCLUDE_DIR})
target_include_directories(${BINDING_NAME} PRIVATE ${PYSIDE_ADDITIONAL_INCLUDES})
target_include_directories(${BINDING_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(${BINDING_NAME} PRIVATE ${INCLUDES})

target_link_libraries(${BINDING_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
        Qt6::Multimedia
        Qt6::MultimediaWidgets
)
target_link_libraries(${BINDING_NAME} PRIVATE ${SHIBOKEN_MODULE_SHARED_LIBRARIES})
target_link_libraries(${BINDING_NAME} PRIVATE ${PYSIDE_SHARED_LIBRARIES})
target_link_libraries(${BINDING_NAME} PRIVATE ${LIBRARY_NAME})

# Adjust the name of generated module.
set_property(TARGET ${BINDING_NAME} PROPERTY PREFIX "")
set_property(TARGET ${BINDING_NAME} PROPERTY OUTPUT_NAME
             "${BINDING_NAME}${PYTHON_EXTENSION_SUFFIX}")
if(WIN32)
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set_property(TARGET ${BINDING_NAME} PROPERTY SUFFIX "_d.pyd")
    else()
        set_property(TARGET ${BINDING_NAME} PROPERTY SUFFIX ".pyd")
    endif()
endif()

# Make sure the linker doesn't complain about not finding Python symbols on macOS.
if(APPLE)
  set_target_properties(${BINDING_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)


# Find and link to the python library.
list(GET PYTHON_LINKING_DATA 0 PYTHON_LIBDIR)
list(GET PYTHON_LINKING_DATA 1 PYTHON_LIB)
find_library(PYTHON_LINK_FLAGS ${PYTHON_LIB} PATHS ${PYTHON_LIBDIR} HINTS ${PYTHON_LIBDIR})
target_link_libraries(${BINDING_NAME} PRIVATE ${PYTHON_LINK_FLAGS})

# Same as CONFIG += no_keywords to avoid syntax errors in object.h due to the usage of the word Slot
target_compile_definitions(${BINDING_NAME} PRIVATE QT_NO_KEYWORDS)

set(WINDOWS_SHIBOKEN_SHARED_LIBRARIES)
if(WIN32)
    # =============================================================================================
    # !!! (The section below is deployment related, so in a real world application you will want to
    # take care of this properly (this is simply to eliminate errors that users usually encounter.
    # =============================================================================================
    # Circumvent some "#pragma comment(lib)"s in "include/pyconfig.h" which might force to link
    # against a wrong python shared library.
    set(PYTHON_VERSIONS_LIST 3 36 37 38 39)
    set(PYTHON_ADDITIONAL_LINK_FLAGS "")

    foreach(VER ${PYTHON_VERSIONS_LIST})
        set(PYTHON_ADDITIONAL_LINK_FLAGS
            "${PYTHON_ADDITIONAL_LINK_FLAGS} /NODEFAULTLIB:\"python${VER}_d.lib\"")
        set(PYTHON_ADDITIONAL_LINK_FLAGS
            "${PYTHON_ADDITIONAL_LINK_FLAGS} /NODEFAULTLIB:\"python${VER}.lib\"")
    endforeach()

    set_target_properties(${BINDING_NAME}
                               PROPERTIES LINK_FLAGS "${PYTHON_ADDITIONAL_LINK_FLAGS}")

    # Compile a list of shiboken shared libraries to be installed, so that
    # the user doesn't have to set the PATH manually to point to the PySide6 package.
    foreach(library_path ${SHIBOKEN_MODULE_SHARED_LIBRARIES})
        string(REGEX REPLACE ".lib$" ".dll" library_path ${library_path})
        file(TO_CMAKE_PATH ${library_path} library_path)
        list(APPEND WINDOWS_SHIBOKEN_SHARED_LIBRARIES "${library_path}")
    endforeach()

    foreach(library_path ${${LIBRARY_NAME}_DEPENDENCIES})
        string(REGEX REPLACE ".lib$" ".dll" library_path ${library_path})
        file(TO_CMAKE_PATH ${library_path} library_path)
        list(APPEND WINDOWS_SHIBOKEN_SHARED_LIBRARIES "${library_path}")
    endforeach()

    # =============================================================================================
    # !!! End of dubious section.
    # =============================================================================================
endif()

message("windowshifsdf " ${WINDOWS_SHIBOKEN_SHARED_LIBRARIES})
message("destination " ${CONSUMER_LIB_DIR})
message("binding " ${BINDING_NAME})
message("libraryName " ${LIBRARY_NAME})

# =============================================================================================
# !!! (The section below is deployment related, so in a real world application you will want to
# take care of this properly with some custom script or tool).
# =============================================================================================
# Install the library and the bindings module into the source folder near the main.py file, so
# that the Python interpeter successfully imports the used module.


install(TARGETS ${BINDING_NAME} ${LIBRARY_NAME}
        LIBRARY DESTINATION ${CONSUMER_LIB_DIR}
        RUNTIME DESTINATION ${CONSUMER_LIB_DIR}
        # INCLUDES DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}
)
install(FILES ${WINDOWS_SHIBOKEN_SHARED_LIBRARIES} DESTINATION ${CONSUMER_LIB_DIR})

# Get the runtime dependencies of the shared library
install(CODE "file(GET_RUNTIME_DEPENDENCIES
                     RESOLVED_DEPENDENCIES_VAR resolved_deps
                     UNRESOLVED_DEPENDENCIES_VAR unresolved_deps
                     LIBRARIES \"${CMAKE_BINARY_DIR}/${LIBRARY_NAME}.lib\")")

# Copy the resolved dependencies to the installation directory
install(CODE "foreach(dep \${resolved_deps})
                  file(INSTALL \"\${dep}\" DESTINATION \"${CONSUMER_LIB_DIR}\")
              endforeach()")

# =============================================================================================
# !!! End of dubious section.
# =============================================================================================

