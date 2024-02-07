from enum import Enum
import ctypes
from ctypes import Structure, c_char_p, c_float, c_void_p, c_long, c_uint8, c_int
import platform


mode = dict(winmode=0) if platform.python_version() >= '3.8' else dict()
lib = ctypes.CDLL('/usr/local/lib/libcpyvesc.so', **mode)


class DataPackage(Structure):
    _fields_ = [
        ('avgMotorCurrent',     c_float),
        ('avgInputCurrent',     c_float),
        ('dutyCycleNow',        c_float),
        ('rpm',                 c_float),
        ('inpVoltage',          c_float),
        ('ampHours',            c_float),
        ('ampHoursCharged',     c_float),
        ('wattHours',           c_float),
        ('wattHoursCharged',    c_float),
        ('tachometer',          c_long),
        ('tachometerAbs',       c_long),
        ('tempMosfet',          c_float),
        ('tempMotor',           c_float),
        ('pidPos',              c_float),
        ('id',                  c_uint8),
        ('mc_fault_code',       c_int)
    ]

    def get_dict(self):
        return dict((f, getattr(self, f)) for f, _ in self._fields_)


class FWversionPackage(Structure):
    _fields_ = [
        ('major',     c_uint8),
        ('minor',     c_uint8)
    ]



class BaudRate(int, Enum):
    B_0         = 0
    B_50        = 1
    B_75        = 2
    B_110       = 3
    B_134       = 4
    B_150       = 5
    B_200       = 6
    B_300       = 7
    B_600       = 8
    B_1200      = 9
    B_1800      = 10
    B_2400      = 11
    B_4800      = 12
    B_9600      = 13
    B_19200     = 14
    B_38400     = 15
    B_57600     = 16
    B_115200    = 17
    B_230400    = 18
    B_460800    = 19



class VescController(object):
    def __init__(self, serial_device: str, rate: BaudRate = BaudRate.B_115200, timeout: int = 50):
        # Declare input and output types for each method you intend to use
        lib.init.argtypes = [c_char_p, c_int, c_int]
        lib.init.restype = c_void_p

        lib.set_rpm.argtypes = [c_void_p, c_float, c_int]
        lib.set_rpm.restype = c_void_p

        lib.set_duty.argtypes = [c_void_p, c_float, c_int]
        lib.set_duty.restype = c_void_p

        lib.set_current.argtypes = [c_void_p, c_float, c_int]
        lib.set_current.restype = c_void_p

        lib.set_brake_current.argtypes = [c_void_p, c_float, c_int]
        lib.set_brake_current.restype = c_void_p

        lib.get_info.argtypes = [c_void_p, c_int]
        lib.get_info.restype = c_void_p

        lib.get_firmware_version.argtypes = [c_void_p, c_int]
        lib.get_firmware_version.restype = c_void_p

        lib.close.argtypes = [c_void_p]
        lib.close.restype = c_void_p

        self.obj = lib.init(serial_device.encode('utf-8'), rate.value, timeout)

    def close(self):
        lib.close(self.obj)

    def set_rpm(self, value: float, can_id: int = 0):
        lib.set_rpm(self.obj, value, can_id)

    def set_duty(self, value: float, can_id: int = 0):
        lib.set_duty(self.obj, value, can_id)

    def set_current(self, value: float, can_id: int = 0):
        lib.set_current(self.obj, value, can_id)

    def set_brake_current(self, value: float, can_id: int = 0):
        lib.set_brake_current(self.obj, value, can_id)

    def get_info(self, can_id: int = 0):
        return DataPackage.from_address(lib.get_info(self.obj, can_id))

    def get_firmware_version(self, can_id: int = 0):
        return FWversionPackage.from_address(lib.get_firmware_version(self.obj, can_id))
