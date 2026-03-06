# Kratos Firmware

By: **York University Rover Team (YURS)** — Lassonde School of Engineering

![Platform](https://img.shields.io/badge/Platform-STM32-blue)
![Language](https://img.shields.io/badge/Language-C%2FC++-orange)
![Communication](https://img.shields.io/badge/Bus-CAN-green)
![System](https://img.shields.io/badge/System-Rover%20Control-lightgrey)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

This repository contains the **embedded firmware** for the **Kratos rover platform**, developed by the **York University Rover Team (YURS)**. The firmware targets **STM32 microcontrollers** and is designed to support modular rover subsystems including IMU, fan control, headlight control and temperature sensing. The goal of this project is to provide a **robust and scalable embedded architecture** for rover control systems. By leveraging real time embedded firmware and a distributed **CAN bus network**, the system enables reliable communication between subsystem controllers while maintaining deterministic timing and low power consumption.

---

## Repository Structure
```
kratos-firmware/
│
├── firmware/
│ └── kratos/
│ ├── Core/ # Application source code
│ ├── Drivers/ # STM32 HAL and CMSIS drivers
│ ├── Middlewares/ # Optional middleware components
│ └── kratos.ioc
│
├── .github/ # CI workflows
├── README.md
└── LICENSE
```

