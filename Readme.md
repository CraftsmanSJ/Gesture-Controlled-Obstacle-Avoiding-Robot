#  Gesture-Controlled Obstacle-Avoiding Robot

A wireless robotic system that combines **gesture-based control** and **autonomous obstacle avoidance** using **Arduino UNO**, **MPU6050**, **nRF24L01**, **HC-SR04**, and **L298N motor drivers**.  
The project demonstrates real-time motion mapping from a transmitter glove to a receiver-controlled robot with autonomous safety features.

---

## Project Overview

This system allows a user to control the movement of a robot simply by tilting their hand.  
The **transmitter unit** captures hand gestures using an **MPU6050 accelerometer + gyroscope sensor**, sends the motion data wirelessly via **nRF24L01**, and the **receiver unit** interprets this data to drive motors accordingly.  

Additionally, the robot uses an **ultrasonic sensor (HC-SR04)** for obstacle detection, allowing it to automatically stop or avoid collisions when an object is detected in front of it.

---

##  Hardware Components

###  Transmitter Module
- Arduino UNO  
- MPU6050 (Accelerometer + Gyroscope Sensor)  
- nRF24L01 Transceiver Module  
- Power Supply (Battery Pack)

###  Receiver Module
- Arduino UNO  
- nRF24L01 Transceiver Module  
- HC-SR04 Ultrasonic Sensor  
- 2 × L298N Motor Drivers  
- 4 × DC Motors  
- Battery Pack (Li-ion 18650 cells)

---

##  Working Principle

1. **Gesture Detection:**  
   The MPU6050 senses orientation changes (tilt/roll/pitch) of the hand.  

2. **Wireless Transmission:**  
   The transmitter Arduino sends motion data via nRF24L01 to the receiver module.  

3. **Motor Control:**  
   The receiver Arduino decodes the gesture signals and drives the motors through L298N motor drivers.
