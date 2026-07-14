#include "tof_hardware/tof_serial.hpp"

#include <cstring>
#include <stdexcept>

#include "tof_hardware/cobsr.h"

TofSerial::~TofSerial()
{
    Close();
}

LibSerial::BaudRate TofSerial::BaudRateFromInt(int baud)
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
    default:
        throw std::invalid_argument("Unsupported baud rate: " + std::to_string(baud));
    }
}

uint8_t TofSerial::CalculateChecksum(uint8_t sensor_id, uint16_t range)
{
    return sensor_id ^
           (range & 0xFF) ^
           ((range >> 8) & 0xFF);
}

void TofSerial::Init(const TofSerialConfig &config)
{
    // Close any previously-open port first so Init() can double as "reopen
    // with new settings" without the caller needing to know that.
    Close();

    port_.Open(config.port);
    port_.SetBaudRate(BaudRateFromInt(config.baud_rate));
    port_.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
    port_.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
    port_.SetParity(LibSerial::Parity::PARITY_NONE);
    port_.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);

    config_ = config;
    rx_buffer_.clear();
}

void TofSerial::Close()
{
    if (port_.IsOpen())
    {
        port_.Close();
    }
    rx_buffer_.clear();
}

bool TofSerial::IsOpen() const
{
    return port_.IsOpen();
}

TofReadStatus TofSerial::ProcessBuffer(TofPacket &out_packet) const
{
    uint8_t decoded[64];

    auto result = cobsr_decode(
        decoded,
        sizeof(decoded),
        rx_buffer_.data(),
        rx_buffer_.size());

    if (result.status != COBSR_DECODE_OK)
    {
        return TofReadStatus::DecodeError;
    }

    if (result.out_len != sizeof(TofPacket))
    {
        return TofReadStatus::DecodeError;
    }

    TofPacket packet;
    std::memcpy(&packet, decoded, sizeof(packet));

    if (packet.checksum != CalculateChecksum(packet.sensor_id, packet.range))
    {
        return TofReadStatus::ChecksumError;
    }

    out_packet = packet;
    return TofReadStatus::Packet;
}

TofReadStatus TofSerial::ReadPacket(TofPacket &out_packet, unsigned int timeout_ms)
{
    char byte;

    try
    {
        port_.ReadByte(byte, timeout_ms);
    }
    catch (const LibSerial::ReadTimeout &)
    {
        return TofReadStatus::NoPacket;
    }
    catch (const std::exception &)
    {
        return TofReadStatus::SerialError;
    }

    const uint8_t b = static_cast<uint8_t>(byte);

    if (b == 0)
    {
        // Delimiter. An empty buffer here just means back-to-back
        // delimiters (or startup noise) -- nothing to decode.
        if (rx_buffer_.empty())
        {
            return TofReadStatus::NoPacket;
        }

        const TofReadStatus status = ProcessBuffer(out_packet);
        rx_buffer_.clear();
        return status;
    }

    rx_buffer_.push_back(b);

    // Guard against an infinitely growing buffer if framing sync is lost.
    if (rx_buffer_.size() > 64)
    {
        rx_buffer_.clear();
    }

    return TofReadStatus::NoPacket;
}