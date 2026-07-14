#pragma once
#include <libserial/SerialPort.h>

LibSerial::BaudRate toLibSerialBaud(uint32_t baud)
{
    switch (baud)
    {
    case 9600:
        return LibSerial::BaudRate::BAUD_9600;
    case 19200:
        return LibSerial::BaudRate::BAUD_19200;
    case 38400:
        return LibSerial::BaudRate::BAUD_38400;
    case 57600:
        return LibSerial::BaudRate::BAUD_57600;
    case 115200:
        return LibSerial::BaudRate::BAUD_115200;
    case 230400:
        return LibSerial::BaudRate::BAUD_230400;
    case 460800:
        return LibSerial::BaudRate::BAUD_460800;
    case 921600:
        return LibSerial::BaudRate::BAUD_921600;
    default:
        throw std::invalid_argument(
            "Unsupported baud rate: " + std::to_string(baud));
    }
}