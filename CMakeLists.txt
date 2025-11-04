cmake_minimum_required(VERSION 3.16)
project(Healthcare_Management_System)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Healthcare_Management_System
    main.cpp
    FileManager.cpp
    PrimaryIndex.cpp
    SecondaryIndex.cpp
    Operations.cpp
)

target_include_directories(Healthcare_Management_System PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
