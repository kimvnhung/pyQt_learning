# Function to loop through include directories for a list of Qt modules
function(find_includes_dir target)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs PACKAGES)

    cmake_parse_arguments(FIND_INCLUDES_DIR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach(QT_MODULE ${FIND_INCLUDES_DIR_PACKAGES})
        get_property(QT_INCLUDE_DIRS TARGET ${QT_MODULE} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
        foreach(INCLUDE_DIR ${QT_INCLUDE_DIRS})
            list(APPEND ${target} "-I${INCLUDE_DIR}")
        endforeach()
    endforeach()
    set(${target} ${${target}} PARENT_SCOPE)
endfunction()
