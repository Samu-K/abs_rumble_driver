cmake_minimum_required(VERSION 3.16)

project(abs_rumble_gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick Network)

qt_standard_project_setup()

qt_add_executable(appabs_rumble_gui
    src/main.cpp
    inc/Backend.h src/Backend.cpp
    inc/ISerial.h
    inc/MockSerial.h
)

target_include_directories(appabs_rumble_gui PRIVATE inc ../abs_rumble_esp/main/inc)

qt_add_qml_module(appabs_rumble_gui
    URI abs_rumble_gui
    VERSION 1.0
    QML_FILES src/qml/main.qml
)

target_link_libraries(appabs_rumble_gui
    PRIVATE Qt6::Gui Qt6::Qml Qt6::Quick Qt6::Network
)

install(TARGETS appabs_rumble_gui
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
