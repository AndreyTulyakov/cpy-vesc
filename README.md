# C-PY-Vesc

It is C++/Python library for Linux systems with limited functionality for controlling VESC motor controllers. 
- Tested on RaspberryPi 4.

Based on libraries:
- [CppLinuxSerial](https://github.com/gbmhunter/CppLinuxSerial)
- [VescUart](https://github.com/SolidGeek/VescUart)


## Installation

1. Install **CppLinuxSerial** library:

    ```shell
    git clone https://github.com/AndreyTulyakov/CppLinuxSerial.git
    cd CppLinuxSerial
    mkdir build/
    cd build
    cmake ..
    make
    sudo make install
    cd ../..
    ```
    
2) Compile CPY-Vesc:

    ```shell
    git clone https://github.com/AndreyTulyakov/cpyvesc.git
    cd cpyvesc
    mkdir build
    cd build/
    cmake ..
    make
    sudo make install
    ```
    
Also if you use python bindings, you can add library to PYTHONPATH:
```bash
export PYTHONPATH=$PYTHONPATH:YOUR_PATH_TO_LIB/cpyvesc
```

    
## Examples:

Warning! Check your VESC uart baud rate settings (default is 115200).


### Python 3

There are simple example of obtaining some VESC controller information, and settings duty control command.

```python
from cpyvesc import VescController, BaudRate
from time import sleep

vesc_controller = VescController(serial_device='/dev/ttyACM0', rate=BaudRate.B_57600, timeout=50)

try:

    fw_data = vesc_controller.get_firmware_version()
    print(f'VESC FIRMWARE VERSION {fw_data.major}.{fw_data.minor}')

    for i in range(100):
        vesc_controller.set_duty(i * 0.005)
        data = vesc_controller.get_info()
        print(f'VESC DATA: \tRPM: {data.rpm} \tVOLTAGE: {data.inpVoltage} \tCURRENT: {data.avgInputCurrent}')
        sleep(0.2)

    vesc_controller.set_current(0)

except Exception as e:
    print("Exception:", str(e))

finally:
    vesc_controller.close()

```


If you want control several VESC's linked by CAN-BUS, you can pass additional argument with target VESC BUS ID (e.g. 87):

```python
fw_data = vesc_controller.get_firmware_version(87)
...
vesc_controller.set_duty(0.25, 87)
```


### C++

```C++
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
```
