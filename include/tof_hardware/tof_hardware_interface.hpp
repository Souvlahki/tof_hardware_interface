#ifndef TOF_HARDWARE_INTERFACE_HPP
#define TOF_HARDWARE_INTERFACE_HPP

#include "hardware_interface/sensor.hpp"

#include "rclcpp/rclcpp.hpp"
#include <libserial/SerialPort.h>

namespace tof_hardware
{

    class TofHardwareInterface : public hardware_interface::SensorInterface
    {
        struct Config
        {
            uint32_t baud_rate;
            uint32_t timeout_ms;
            std::string port;
            uint32_t sample_rate;
        };

    public:
        hardware_interface::CallbackReturn
        on_activate(const rclcpp_lifecycle::State &previous_state) override;

        hardware_interface::CallbackReturn
        on_deactivate(const rclcpp_lifecycle::State &previous_state) override;

        hardware_interface::CallbackReturn
        on_init(const hardware_interface::HardwareComponentInterfaceParams &params) override;

        hardware_interface::return_type
        read(const rclcpp::Time &time, const rclcpp::Duration &period) override;

    private:
        Config cfg_;
        LibSerial::SerialPort serial_;
    };

} // namespace tof_hardware

#endif