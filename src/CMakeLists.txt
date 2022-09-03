
add_subdirectory(external)

if(TEST)
        add_subdirectory(../tests ./tests)
else()
        add_subdirectory(analyser) 
        add_subdirectory(machine)
        if(IS_GUI)     
                add_subdirectory(ui)
        else()
                add_subdirectory(emulator)
        endif()
endif()