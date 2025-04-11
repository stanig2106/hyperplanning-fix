cmake_minimum_required(VERSION 3.0)
project(hyperplanning-fix)

set(CMAKE_CXX_STANDARD 14)

add_executable(hyperplanning-fix src/main.cpp)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 23598e8a867934280db1f3bd3fe8cfa268a8d378) # 1.9.7
FetchContent_MakeAvailable(cpr)


SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS filesystem system thread regex program_options date_time)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF (Boost_FOUND)

target_link_libraries(hyperplanning-fix ${Boost_LIBRARIES} cpr::cpr)
