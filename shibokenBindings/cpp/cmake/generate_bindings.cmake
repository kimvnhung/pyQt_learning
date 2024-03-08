# Generate binding file
# Define input and output files
set(INPUT_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/bindings_template.xml)
set(OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/bindings.xml)

# Read the template file
file(READ ${INPUT_TEMPLATE} TEMPLATE_CONTENT)

# Replace placeholders with actual values

string(REPLACE "@PROJECT_NAME@" "${PROJECT_NAME}" MODIFIED_CONTENT "${TEMPLATE_CONTENT}")
string(CONCAT MODIFIED_CONTENT "<!--\n// This is auto-generated file\n-->" "${TEMPLATE_CONTENT}")

# message("OUPTUT file " "${OUTPUT_FILE}")
# Write the modified content to the output file
file(WRITE ${OUTPUT_FILE} "${MODIFIED_CONTENT}")