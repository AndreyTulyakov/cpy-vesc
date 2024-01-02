#include <thread>

#include "CppLinuxSerial/SerialPort.hpp"
#include "VescUart.h"


using namespace std::chrono_literals;
using namespace mn::CppLinuxSerial;

extern "C"
{
    __attribute__((visibility("default"))) VescUart* init(const char* serial_device, BaudRate baud_rate, int timeout) {
        SerialPort* serial = new SerialPort(serial_device, baud_rate, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
        serial->SetTimeout(timeout);
        serial->Open();
        std::this_thread::sleep_for(10ms);
        return new VescUart(serial);
    }

    __attribute__((visibility("default"))) void set_rpm(VescUart *self, float value,  int can_id = 0) {self->setRPM(value, can_id);}
    __attribute__((visibility("default"))) void set_duty(VescUart *self, float value,  int can_id = 0) {self->setDuty(value, can_id);}
    __attribute__((visibility("default"))) void set_current(VescUart *self, float value,  int can_id = 0) {self->setCurrent(value, can_id);}
    __attribute__((visibility("default"))) void set_brake_current(VescUart *self, float value,  int can_id = 0) {self->setBrakeCurrent(value, can_id);}

    __attribute__((visibility("default"))) VescUart::DataPackage* get_info(VescUart *self, int can_id = 0) {
        self->getVescValues(can_id);
        return &self->data;
    }

    __attribute__((visibility("default"))) VescUart::FWversionPackage* get_firmware_version(VescUart *self, int can_id = 0) {
        self->getFWversion(can_id);
        return &self->fw_version;
    }

    __attribute__((visibility("default"))) void close(VescUart *self) {
        SerialPort* serial = self->getSerial();
        serial->Close();
        delete self;
        delete serial;
    }


}
