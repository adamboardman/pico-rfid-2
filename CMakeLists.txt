add_library(pico_rfid_2 INTERFACE)

target_sources(pico_rfid_2 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/source/rfid_2.cpp
        )

target_include_directories(pico_rfid_2 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/api)
target_link_libraries(pico_rfid_2 INTERFACE pico_interfaces pico_stdlib hardware_gpio hardware_i2c)