cmake_minimum_required(VERSION 3.14)

project(Gawain VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_ENABLE_EXPORTS ON)

find_package(fmt CONFIG REQUIRED)

add_library(${PROJECT_NAME}
        include/Gawain/API/Common/Common.hpp
        include/Gawain/API/Common/StockPacket.hpp
        include/Gawain/API/Adaptor/Adaptor.hpp
        include/Gawain/API/Strategy/Strategy.hpp
        include/Gawain.hpp
        src/StockPacket.cpp
        src/Common.cpp
        )
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
