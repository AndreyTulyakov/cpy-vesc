#include <chrono>
#include <iostream>

#include <CppLinuxSerial/SerialPort.hpp>
#include <thread>

#include "src/vesc_uart.h"


using namespace std;
using namespace std::chrono_literals;
using namespace mn::CppLinuxSerial;


int main(int argc, char* argv[]) {
    string serial_device = "/dev/ttyACM0";

    if (argc > 1) {
        serial_device = argv[1];
    }

    SerialPort serialPort(serial_device, BaudRate::B_57600, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);

    serialPort.SetTimeout(50);
    serialPort.Open();

    std::this_thread::sleep_for(10ms);

    VescUart controller(&serialPort);


    for(int i=0; i<100; i++) {
        if (controller.getVescValues() ) {

            VescUart::DataPackage data = controller.data;
            std::cout << "RPM:" << data.rpm << std::endl;
            std::cout << "VOLTAGE:" << data.inpVoltage << std::endl;
            std::cout << "AMP*H:" << data.ampHours << std::endl;
            std::cout << "CURRENT:" << data.avgInputCurrent << std::endl;

            std::cout << std::endl;
          }
          else
          {
            std::cout << "Failed to get data!" << std::endl;
          }

        if (controller.getVescValues(86) ) {
            VescUart::DataPackage data = controller.data;
            std::cout << "2 RPM:" << data.rpm << std::endl;
            std::cout << "2 VOLTAGE:" << data.inpVoltage << std::endl;
            std::cout << "2 AMP*H:" << data.ampHours << std::endl;
            std::cout << "2 CURRENT:" << data.avgInputCurrent << std::endl;

            std::cout << std::endl;
        }
        else
        {
          std::cout << "Failed to get data second vesc!" << std::endl;
        }

        controller.setDuty(0.01f + 0.001f * i);
        std::this_thread::sleep_for(50ms);
    }

    controller.setCurrent(0);

    std::this_thread::sleep_for(50ms);

    // Close the serial port
    serialPort.Close();
}
