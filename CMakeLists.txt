cmake_minimum_required(VERSION 3.5)

find_package(CppLinuxSerial REQUIRED)

project(cpyvesc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -fPIC")


add_library(cpyvesc SHARED
    src/buffer.cpp src/buffer.h src/crc.cpp src/crc.h src/datatypes.h src/VescUart.cpp src/VescUart.h
    src/python_wrapper.cpp
)


include(GNUInstallDirs)
install(TARGETS cpyvesc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(cpyvesc CppLinuxSerial)
