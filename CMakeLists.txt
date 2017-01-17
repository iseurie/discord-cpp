CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
if(WINDOWS)
    set(CMAKE_0.1_COMPILER cl.exe)
elseif(UNIX)
    set(CMAKE_0.1_COMPILER g++)
endif()

project(discord-cpp 0.1 CXX)

file(GLOB CORE_SOURCES 
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cc)
add_library(core STATIC ${CORE_SOURCES})
