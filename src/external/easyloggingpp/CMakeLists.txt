set(test False)

FetchContent_MakeAvailable(easyloggingpp)

file(COPY ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc DESTINATION ${CMAKE_SOURCE_DIR}/src/external/easyloggingpp/)
file(COPY ${easyloggingpp_SOURCE_DIR}/src/easylogging++.h DESTINATION ${CMAKE_SOURCE_DIR}/src/external/easyloggingpp/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DELPP_NO_DEFAULT_LOG_FILE")

add_library(easyloggingpp STATIC easylogging++.cc)