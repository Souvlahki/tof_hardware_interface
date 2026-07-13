# ToF Sensor Hardware Interface

A ROS 2 hardware interface for integrating Time-of-Flight (ToF) distance sensors with the `ros2_control` framework.

> **Project Status:** 🚧 Work in Progress

The hardware interface establishes the communication infrastructure between a ToF sensor and ROS 2. At the current stage, the project includes the hardware interface structure and initialization logic. Sensor data acquisition through the `read()` method is still under development.

---

## Features

* ROS 2 `ros2_control` hardware interface
* Serial communication infrastructure
* Hardware lifecycle support
* Configurable serial port and baud rate
* Designed to support one or more ToF sensors
* Clean and modular C++ implementation

---

## Current Status

| Feature                        | Status         |
| ------------------------------ | -------------- |
| Hardware interface             | ✅ Implemented  |
| Initialization (`on_init`)     | ✅ Implemented  |
| Configuration (`on_configure`) | ✅ Implemented  |
| Activation/Deactivation        | ✅ Implemented  |
| Serial communication           | 🚧 In Progress  |
| `read()`                       | 🚧 In Progress |
| Sensor state publishing        | 🚧 Planned     |
| Diagnostics                    | 📅 Planned     |

---

## Repository Structure

```text
.
├── include/
│   └── tof_hardware_interface/
├── src/
│   ├── tof_hardware_interface.cpp
│   └── serial_interface.cpp
├── launch/
├── config/
├── urdf/
├── CMakeLists.txt
├── package.xml
└── README.md
```

---

## Requirements

* Ubuntu
* ROS 2
* `ros2_control`
* `hardware_interface`
* C++17
* CMake

---

## Build

```bash
colcon build --packages-select<package_name>
source install/setup.bash
```

---

## Usage

Launch the hardware interface using your robot launch file or controller manager configuration.

Example:

```bash
ros2 launch <package_name>
    <launch_file>.launch.py
```

---

## Planned `read()` Implementation

The upcoming implementation will:

* Read range measurements from the ToF sensor
* Parse incoming serial data
* Validate received measurements
* Update hardware state interfaces
* Handle communication errors gracefully
* Support automatic recovery after communication failures

---

## Future Improvements

* Support multiple ToF sensors
* Sensor diagnostics
* Configurable update rate
* Timestamp synchronization
* Error recovery and reconnection
* Unit and integration tests
* Continuous Integration (CI)

---

## License

This project is licensed under the MIT License.

---

## Author

Developed by Muhammed Osama Tarakji.
