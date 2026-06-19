cmake_minimum_required(VERSION 3.21)
include(FetchContent)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if (WIN32)
    add_compile_definitions(WIN32)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()

project(osc-gamemode VERSION 1.0.0)

# Add all source files inside src (recursively)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# ---- oscpack ----
FetchContent_Declare(oscpack
    GIT_REPOSITORY https://github.com/RossBencina/oscpack.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
FetchContent_Populate(oscpack)

if (WIN32)
    set(OSCPACK_IP_PLATFORM_DIR "${oscpack_SOURCE_DIR}/ip/win32")
else()
    set(OSCPACK_IP_PLATFORM_DIR "${oscpack_SOURCE_DIR}/ip/posix")
endif()

target_sources(${PROJECT_NAME} PRIVATE
    ${oscpack_SOURCE_DIR}/ip/IpEndpointName.cpp
    ${OSCPACK_IP_PLATFORM_DIR}/NetworkingUtils.cpp
    ${OSCPACK_IP_PLATFORM_DIR}/UdpSocket.cpp
    ${oscpack_SOURCE_DIR}/osc/OscTypes.cpp
    ${oscpack_SOURCE_DIR}/osc/OscOutboundPacketStream.cpp
    ${oscpack_SOURCE_DIR}/osc/OscReceivedElements.cpp
)

set_source_files_properties(
    ${oscpack_SOURCE_DIR}/ip/IpEndpointName.cpp
    ${OSCPACK_IP_PLATFORM_DIR}/NetworkingUtils.cpp
    ${OSCPACK_IP_PLATFORM_DIR}/UdpSocket.cpp
    ${oscpack_SOURCE_DIR}/osc/OscTypes.cpp
    ${oscpack_SOURCE_DIR}/osc/OscOutboundPacketStream.cpp
    ${oscpack_SOURCE_DIR}/osc/OscReceivedElements.cpp
    PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

target_include_directories(${PROJECT_NAME} PRIVATE ${oscpack_SOURCE_DIR})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 winmm)
endif()
# ---- end oscpack ----

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})