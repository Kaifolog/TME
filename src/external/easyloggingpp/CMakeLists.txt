FetchContent_MakeAvailable(easyloggingpp)

file(COPY ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc DESTINATION ${CMAKE_SOURCE_DIR}/src/external/easyloggingpp/)
file(COPY ${easyloggingpp_SOURCE_DIR}/src/easylogging++.h DESTINATION ${CMAKE_SOURCE_DIR}/src/external/easyloggingpp/)

add_library(easyloggingpp STATIC easylogging++.cc)