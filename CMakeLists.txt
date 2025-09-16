cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(mqttwin LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "c:/myapp/work_about/QT/QT/6.9.2/mingw_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 COMPONENTS Widgets REQUIRED) # Qt COMPONENTS

add_subdirectory(src/src/qmqttclient)

aux_source_directory(./src srcs)
add_executable(${PROJECT_NAME}
    # WIN32 #如果需要终端把这里注释即可
    ${srcs} 
) 
target_link_libraries(${PROJECT_NAME} Qt6::Widgets mqttclient) # Qt6 Shared Library