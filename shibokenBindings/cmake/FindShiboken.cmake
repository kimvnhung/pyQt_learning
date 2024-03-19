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