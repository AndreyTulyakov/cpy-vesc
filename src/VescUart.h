#ifndef _VESCUART_h
#define _VESCUART_h

#include <chrono>
#include <vector>
#include <CppLinuxSerial/SerialPort.hpp>

#include "datatypes.h"



using namespace mn::CppLinuxSerial;


class VescUart
{
public:
	/** Struct to store the telemetry data returned by the VESC */
    struct DataPackage {
        float avgMotorCurrent;
        float avgInputCurrent;
        float dutyCycleNow;
        float rpm;
        float inpVoltage;
        float ampHours;
        float ampHoursCharged;
        float wattHours;
        float wattHoursCharged;
        long tachometer;
        long tachometerAbs;
        float tempMosfet;
        float tempMotor;
        float pidPos;
        uint8_t id;
        mc_fault_code error;
	};

    struct FWversionPackage {
        uint8_t major;
        uint8_t minor;
    };

	//Timeout - specifies how long the function will wait for the vesc to respond
	const uint32_t _TIMEOUT;

	public:
		/**
		 * @brief      Class constructor
		 */
        VescUart(SerialPort* serial, uint32_t timeout_ms=100);

        // Variabel to hold measurements returned from VESC
        DataPackage data;

        // Variable to hold firmware version
        FWversionPackage fw_version; 


        /**
         * @brief      Populate the firmware version variables
         *
         * @return     True if successfull otherwise false
         */
        bool getFWversion(void);
        bool getFWversion(uint8_t canId);

        /**
         * @brief      Sends a command to VESC and stores the returned data
         *
         * @return     True if successfull otherwise false
         */
        bool getVescValues(void);
        bool getVescValues(uint8_t canId);

        /**
         * @brief      Set the current to drive the motor
         * @param      current  - The current to apply
         */
        void setCurrent(float current);

        /**
         * @brief      Set the current to drive the motor
         * @param      current  - The current to apply
         * @param      canId  - The CAN ID of the VESC
         */
        void setCurrent(float current, uint8_t canId);

        /**
         * @brief      Set the current to brake the motor
         * @param      brakeCurrent  - The current to apply
         */
        void setBrakeCurrent(float brakeCurrent);

        /**
         * @brief      Set the current to brake the motor
         * @param      brakeCurrent  - The current to apply
         * @param      canId  - The CAN ID of the VESC
         */
        void setBrakeCurrent(float brakeCurrent, uint8_t canId);


        /**
         * @brief      Set the rpm of the motor
         * @param      rpm  - The desired RPM (actually eRPM = RPM * poles)
         */
        void setRPM(float rpm);		

        /**
         * @brief      Set the rpm of the motor
         * @param      rpm  - The desired RPM (actually eRPM = RPM * poles)
         * @param      canId  - The CAN ID of the VESC
         */
        void setRPM(float rpm, uint8_t canId);

        /**
         * @brief      Set the duty of the motor
         * @param      duty  - The desired duty (0.0-1.0)
         */
        void setDuty(float duty);

        /**
         * @brief      Set the duty of the motor
         * @param      duty  - The desired duty (0.0-1.0)
         * @param      canId  - The CAN ID of the VESC
         */
        void setDuty(float duty, uint8_t canId);

        /**
         * @brief      Send a keepalive message
         */
        void sendKeepalive(void);

        /**
         * @brief      Send a keepalive message
         * @param      canId  - The CAN ID of the VESC
         */
        void sendKeepalive(uint8_t canId);

        SerialPort *getSerial();


	private: 
        SerialPort* serialPort = NULL;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

		/**
		 * @brief      Packs the payload and sends it over Serial
		 *
		 * @param      payload  - The payload as a unit8_t Array with length of int lenPayload
		 * @param      lenPay   - Length of payload
		 * @return     The number of bytes send
		 */
		int packSendPayload(uint8_t * payload, int lenPay);

		/**
		 * @brief      Receives the message over Serial
		 *
		 * @param      payloadReceived  - The received payload as a unit8_t Array
		 * @return     The number of bytes receeived within the payload
		 */
		int receiveUartMessage(uint8_t * payloadReceived);

		/**
		 * @brief      Verifies the message (CRC-16) and extracts the payload
		 *
		 * @param      message  - The received UART message
		 * @param      lenMes   - The lenght of the message
		 * @param      payload  - The final payload ready to extract data from
		 * @return     True if the process was a success
		 */
        bool unpackPayload(std::vector<uint8_t> &message, int lenMes, uint8_t * payload);

		/**
		 * @brief      Extracts the data from the received payload
		 *
		 * @param      message  - The payload to extract data from
		 * @return     True if the process was a success
		 */
		bool processReadPacket(uint8_t * message);

        clock_t millis();

};

#endif
