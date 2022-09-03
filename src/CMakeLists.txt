
add_subdirectory(external)

if(TEST)
        add_subdirectory(../tests ./tests)
else()
        add_subdirectory(analyser) 
        add_subdirectory(machine)
        if(GUI)     
                add_subdirectory(gui)
        else()
                add_subdirectory(cli)
        endif()
endif()