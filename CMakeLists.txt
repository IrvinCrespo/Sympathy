# cmake version to be used
cmake_minimum_required( VERSION 3.0 )

# project name
project(Sympathy)

# flags
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    message(STATUS "C++11 support has been enabled by default.")


# files

# include
include_directories(./include ./include/${CMAKE_PROJECT_NAME} ./src)

# target
add_executable( ${CMAKE_PROJECT_NAME} 
./src/main.cpp 
./include/${CMAKE_PROJECT_NAME}/Server/Server.cpp
)

if(WIN32)
  target_link_libraries(${CMAKE_PROJECT_NAME} wsock32 ws2_32)
endif()


#install(TARGETS LS DESTINATION bin)
