
add_subdirectory(external)

if(AUTOTEST)
        add_subdirectory(../tests ./tests)
else()
        add_subdirectory(translator) 
        add_subdirectory(machine)
        if(GUI)     
                add_subdirectory(gui)
        else()
                add_subdirectory(cli)
        endif()
endif()