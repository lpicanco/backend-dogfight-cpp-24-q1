cmake_minimum_required(VERSION 3.18)
project(backend_dogfight_cpp_24_q1)

set(CMAKE_CXX_STANDARD 20)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# add dependencies
include(FetchContent)
include(cmake/CPM.cmake)

set(RAPIDJSON_BUILD_TESTS OFF)
CPMAddPackage(
        NAME rapidjson
        VERSION master
        GITHUB_REPOSITORY Tencent/rapidjson
        GIT_TAG 3f73edae00aba5b0112a80b4d41e6f1ff7d92a3d
)

set(BUILD_TESTING OFF)
set(BUILD_TEST OFF)

# Drogon
set(BUILD_EXAMPLES OFF)
set(BUILD_CTL OFF)
set(BUILD_DOC OFF)
set(BUILD_BROTLI OFF)
set(LIBPQ_SUPPORTS_BATCH_MODE ON)
CPMAddPackage(
        NAME drogon
        VERSION 1.9.3
        GITHUB_REPOSITORY drogonframework/drogon
        GIT_TAG v1.9.3
)
set(USE_POSTGRESQL ON)

find_package(PostgreSQL REQUIRED)
FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG        7.8.1
)
FetchContent_MakeAvailable(libpqxx)

include_directories(${libpqxx_SOURCE_DIR}/include)
include_directories(${rapidjson_SOURCE_DIR}/include)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.*")

add_executable(backend-dogfight-cpp-24-q1 ${SOURCES})
target_link_libraries(backend-dogfight-cpp-24-q1 PRIVATE  pqxx pthread PostgreSQL::PostgreSQL drogon)
