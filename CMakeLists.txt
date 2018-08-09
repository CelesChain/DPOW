cmake_minimum_required(VERSION 3.10)

project(pow)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libraries/fc/CMakeModules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

include( GNUInstallDirs )
include( VersionMacros )
include( SetupTargetMacros )
include( InstallDirectoryPermissions )
include( MASSigning )

FIND_PACKAGE(Boost 1.67 REQUIRED COMPONENTS
        thread
        date_time
        filesystem
        system
        program_options
        signals
        serialization
        chrono
        unit_test_framework
        context
        locale
        iostreams)

add_executable(main main.cpp)

target_link_libraries(main pow)

add_subdirectory(libraries)
