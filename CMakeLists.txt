cmake_minimum_required(VERSION 3.5)

project(Gawain VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_ENABLE_EXPORTS ON)

find_package(fmt CONFIG REQUIRED)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(${PROJECT_NAME}_OPTIONS INTERFACE)
target_compile_features(${PROJECT_NAME}_OPTIONS INTERFACE cxx_std_20)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if (ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(${PROJECT_NAME}_OPTIONS INTERFACE -ftime-trace)
    endif ()
endif ()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(${PROJECT_NAME}_WARNINGS INTERFACE)

# enable cache system
set(ENABLE_CACHE OFF)
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME}_WARNINGS)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(${PROJECT_NAME}_WARNINGS)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
    # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
    #
    # consider breaking this out per project as necessary
    target_precompile_headers(
            ${PROJECT_NAME}_OPTIONS
            INTERFACE
            <vector>
            <string>
            <map>
            <utility>)
endif ()


add_library(${PROJECT_NAME}
        include/Gawain/API/Common/Common.hpp
        include/Gawain/API/Common/StockPacket.hpp
        include/Gawain/API/Adaptor/Adaptor.hpp
        include/Gawain/API/Strategy/Strategy.hpp
        include/Gawain.hpp
        src/StockPacket.cpp
        src/Common.cpp
        include/Gawain/API/StreamData/StreamHelper.hpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME} INTERFACE
        ${PROJECT_NAME}_OPTIONS
        ${PROJECT_NAME}_WARNINGS
)