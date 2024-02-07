cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

find_package(CppLinuxSerial REQUIRED)

project(cpyvesc LANGUAGES CXX)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -fPIC")


add_library(cpyvesc SHARED
    src/buffer.cpp src/buffer.h src/crc.cpp src/crc.h src/datatypes.h src/vesc_uart.cpp src/vesc_uart.h
    src/python_wrapper.cpp
)


include(GNUInstallDirs)
install(TARGETS cpyvesc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(cpyvesc CppLinuxSerial)
