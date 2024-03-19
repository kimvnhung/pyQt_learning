macro(configure_files srcDir destDir filter)
    # message(STATUS "Configuring directory ${destDir} from ${srcDir} with filter ${filter}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} "${srcDir}/*")

    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})

        if(NOT IS_DIRECTORY ${srcTemplatePath})
            # message(STATUS "Configuring file ${templateFile}")
            string(FIND ${templateFile} ${filter} RESULT)
            if(RESULT EQUAL -1)
                continue()
            endif()
            configure_file(
                ${srcTemplatePath}
                ${destDir}/${templateFile}
                @ONLY)
        else()
            configure_files(${srcTemplatePath} ${destDir}/${templateFile} ${filter})
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)
