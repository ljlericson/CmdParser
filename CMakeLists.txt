cmake_minimum_required(VERSION 3.30)

# Vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(CmdParser VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(nlohmann_json CONFIG REQUIRED)


# --- Executable ---
add_executable(CmdParser)

# Sources
file(GLOB_RECURSE CmdParser_SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)
target_sources(CmdParser PRIVATE 
	${CmdParser_SOURCES}
)

# Includes
target_include_directories(CmdParser PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(CmdParser PRIVATE
    nlohmann_json::nlohmann_json
)

# --- Compiler warnings ---
if(MSVC)
    target_compile_options(CmdParser PRIVATE
        /W3
        /MP
        /WX
        /ZI
    )
else()
    target_compile_options(CmdParser PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic
    )
endif()

# --- Visual Studio niceties ---
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CmdParser)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${CmdParser_SOURCES})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
