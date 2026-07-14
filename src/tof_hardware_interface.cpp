#include "tof_hardware/tof_hardware_interface.hpp"
#include "tof_hardware/helpers.hpp"

namespace tof_hardware
{
    hardware_interface::CallbackReturn
    TofHardwareInterface::on_init(const hardware_interface::HardwareComponentInterfaceParams &params)
    {

        if (hardware_interface::SensorInterface::on_init(params) != hardware_interface::CallbackReturn::SUCCESS)
            return hardware_interface::CallbackReturn::ERROR;

        const auto &info_ = params.hardware_info;

        cfg_.port = info_.hardware_parameters.at("port");
        cfg_.baud_rate = std::stoi(info_.hardware_parameters.at("baud_rate"));
        cfg_.timeout_ms = std::stoi(info_.hardware_parameters.at("timeout_ms"));
        cfg_.sample_rate = std::stoi(info_.hardware_parameters.at("sample_rate"));

        return hardware_interface::CallbackReturn::SUCCESS;
    }

    hardware_interface::CallbackReturn
    TofHardwareInterface::on_activate(const rclcpp_lifecycle::State &previous_state)
    {
        serial_.Open(cfg_.port);
        serial_.SetBaudRate(toLibSerialBaud(cfg_.baud_rate));

        serial_.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        serial_.SetParity(LibSerial::Parity::PARITY_NONE);
        serial_.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
        serial_.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        return hardware_interface::CallbackReturn::SUCCESS;
    }

    hardware_interface::CallbackReturn
    TofHardwareInterface::on_deactivate(const rclcpp_lifecycle::State &previous_state)
    {
        if (serial_.IsOpen())
            serial_.Close();

        return hardware_interface::CallbackReturn::SUCCESS;
    }

    hardware_interface::return_type
    TofHardwareInterface::read(const rclcpp::Time &time, const rclcpp::Duration &period)
    {
        // read
    }
}

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(tof_hardware::TofHardwareInterface, hardware_interface::SensorInterface)