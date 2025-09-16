add_subdirectory(qtmqtt)
find_package(Qt6 COMPONENTS Core Network REQUIRED)
aux_source_directory(. WINDOW_MQTT)
add_library(mqttclient STATIC ${WINDOW_MQTT})
set_target_properties(mqttclient PROPERTIES 
                                    AUTOMOC ON
                                    AUTORCC ON
                                    AUTOUIC ON)
target_include_directories(mqttclient
    PUBLIC
    .
)
target_link_libraries(mqttclient 
    Qt6::Widgets
    Qt6::Core
    Qt6::Network
    Mqtt)