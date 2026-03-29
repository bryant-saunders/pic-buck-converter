# pic-buck-converter
This is a PIC16F1619 based adjustable Constant On-Time (COT) Pulse Frequency Modulation (PFM) buck converter board. I designed this project to learn more about various DC-DC converter topologies using discrete components and to prove that microcontrollers can be utilized for complex control loops given the right peripheral modules

<img src="https://github.com/user-attachments/assets/4666e9aa-f1a4-4ee6-bea2-fcddc605fff8" width="300px">


## Features

* Buttons offer ability to fine tune output volatage in 80mV and 1V increments
* 6-20V input range. Input voltage should be at least 3V's higher than the expected output voltage
* Reverse polarity protection diode on the input
* Up to 3A output current
* USB output connector for convenience

## Technical details 

* Average conversion efficiency: <b>80.6%</b>
    * 10V in, 5V out @250mA:  73.5%
    * 10V in, 5V out @1000mA: 82.0%
    * 10V in, 5V out @1500mA: 93.7%
 * Line Regulation: 50mV typical at Vout = 5V (Vin = 8-18V)
 * Load Regulation: 36mV/A (VIN=12V, VOUT=5V)
 * Ripple: <100mVp-p (Measured up to 1.5A load)
 * Quiescent Current: (No load power draw, includes power indicator LED) <10mA

## How it works

The power conversion process begins with the input voltage entering through a Schottky diode for reverse polarity protection, which then distributes power to the switching stage and a dedicated LDO regulator (MIC2954). This regulator provides a precise five-volt rail to the PIC16F1619 microcontroller, ensuring that the control logic and internal references remain stable even as the main input voltage fluctuates. Within the microcontroller, an internal DAC sets a reference voltage that represents the desired output. This reference is compared against a scaled-down version of the actual output voltage, which is monitored through the resistive divider connected to the inverting input of the internal comparator module. Because the design requires at least three volts of headroom between the input and output, the system has sufficient electrical "pressure" to rapidly recharge the output stage during each switching cycle.

The regulation itself is managed through a fast control loop that determines exactly when to add energy to the circuit. When the comparator detects that the output voltage has drifted below the target threshold set by the DAC, the microcontroller fires a fixed five-microsecond pulse. This signal drives an N-channel logic-level MOSFET that acts as a high-speed level shifter, pulling the gate of the main P-channel MOSFET toward ground to turn it on. This driver configuration is necessary to avoid complex bootstrap circuits. This pulse-frequency modulation approach ensures that the board only uses the energy necessary to maintain the setpoint, automatically increasing the pulse rate as the load output grows.

During each five-microsecond pulse, current flows through the P-channel MOSFET and into a 150-microhenry power inductor, where energy is stored in a magnetic field while simultaneously charging a large 680-microfarad output capacitor. When the MOSFET turns off at the end of the pulse, the magnetic field in the inductor collapses, forcing the stored energy through a freewheeling Schottky diode to continue supplying the load. The large output capacitor acts as a reservoir, smoothing these individual bursts of energy into a steady DC voltage. By balancing the timing of these pulses against the feedback from the comparator, the system achieves a stable output with relativly low ripple, consistent regulation and low quiescent current

## Building/running your own

The project includes the schematic, gerber file and BOM if you choose to build your own board. The design uses thru-hole components exclusively, so you could also assemble the circuit on a breadboard if you do not want to order PCBs. Building this circuit on a breadboard will introduce more noise into the system, but is good to demonstrate the basic functionality. 

On the software side, you will need a computer that can run the MPLAB IDE from Microchip. You will also need to install the XC8 C compiler and have access to a programmer that supports the PIC16F1619 such as the MPLAB SNAP or PicKit 4

## Notes

This converter was NOT designed for any kind of production use, but rather as a learning experience. There was little effort to minimize or measure EMF emited from the board, and the particular topology (PFM) compounds the interference issue. While basic safety measures were taken in the design, this board has NOT been tested for every failure condition. <b>Using this board/project/code is at your own risk!</b>


