cmake_minimum_required(VERSION 3.16)
project(UltrasonicHost VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort Charts Sql)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/serialport.cpp
    src/datamanager.cpp
    src/chartwidget.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/serialport.h
    src/datamanager.h
    src/chartwidget.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::SerialPort
    Qt6::Charts
    Qt6::Sql
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
