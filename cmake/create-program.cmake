find_program(chmod_command chmod)

function(create_program target_name linker_script)
    add_executable(${target_name})
    target_link_options(${target_name} PRIVATE
        -nostartfiles
        "SHELL:-T ${CMAKE_SOURCE_DIR}/${linker_script}"
        -Wl,--no-warn-rwx-segments
        -Wl,--orphan-handling=warn)
    set_target_properties(${target_name} PROPERTIES LINK_DEPENDS "${CMAKE_SOURCE_DIR}/${linker_script}")
    add_custom_command(TARGET ${target_name} POST_BUILD COMMAND "${chmod_command}" a-x $<TARGET_FILE:${target_name}>)
endfunction()
