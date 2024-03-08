if(DEFINED PROJECT_NAME)
    message("Project name: " "${PROJECT_NAME}")
endif()

function(generate_binding WORKING_DIR TEMPLATE_NAME OUTPUT_NAME)
    # Generate binding file
    # Define input and output files
    set(INPUT_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/${TEMPLATE_NAME}.xml)
    set(OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_NAME}.xml)

    # Read the template file
    file(READ ${INPUT_TEMPLATE} TEMPLATE_CONTENT)

    # Replace placeholders with actual values

    string(REPLACE "LIB_NAME" ${PROJECT_NAME} MODIFIED_CONTENT ${TEMPLATE_CONTENT})
    string(CONCAT MODIFIED_CONTENT "<!--\n// This is auto-generated file\n-->\n\n" "${TEMPLATE_CONTENT}")

    # message("OUPTUT file " "${OUTPUT_FILE}")
    # Write the modified content to the output file
    file(WRITE ${OUTPUT_FILE} "${MODIFIED_CONTENT}")
    message(${MODIFIED_CONTENT})
endfunction()
