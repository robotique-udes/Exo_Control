# Exo_Control

## Table of Contents

- [Exo\_Control](#exo_control)
  - [Table of Contents](#table-of-contents)
  - [Description](#description)
  - [Hardware](#hardware)
  - [Getting Started](#getting-started)
  - [Class description](#class-description)
  - [Testing](#testing)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)
  - [Authors](#authors)

## Description

**BioGénius** is a technical project within the Robotique UdeS group that focuses on designing a lightweight and powerful exoskeleton system. This innovative system provides support to the legs, hips, and back, making walking and load handling easier. Notably, our prototype led the team to secure the first-place position at the Applied Collegiate Exoskeleton (ACE) Competition in the United States in both 2022 and 2023.

Founded in the fall of 2019 by two bioengineering enthusiasts, BioGénius has grown into a team of over fifteen members, bringing together expertise from various engineering disciplines and cohorts. This project represents a pioneering endeavor at the undergraduate engineering level in Quebec.

## Hardware

Our system comprises the following key hardware components:

- **Touch Screen (HMI)**: NX3224F024_11
- **Microcontroller**: ESP32
- **Inertial Measurement Units**: BNO085
- **Motor Drives**: DBH-15

## Getting Started

Getting started with our project is a straightforward process. Simply clone the repository.

## Class description

- main: 
  - Not a class but some things are good to keep in mind:
  - The main is separated in 3 blocs: test, logic and printing
    - Test blocs need to be filled with unit tests for every component, will be commented when not in the process of testing 
    - Logic bloc controls normal operation for the exo, needs to be active when using
    - Printing bloc is for debugging, keep commented otherwise
  - Setup starts IMUs and their wifi functions, comment either of those lines if not needed when testing
- motorControl:
  - This class orchestrates most of the operation in the exo and use the info it receive from various sensors to calculate the needed current the motors need
  - Most of the class is either mathematics or fetching info from other classes
  - Most other classes are called by this one
- imu:
  - BNO055 logic class, used to fetch information from our IMUs, converting it to angles, and sending it to motorControl
  - We use mainly the adafruit bno055 library here
- multiplex:
  - MUX control class composed of simple functions used to switch MUX chanels
- bnoMultiplex:
  - Bridge between the imu and multiplex classes
  - Used to associate any IMUs to an ID from it's constructor and switch MUX channel automaticaly when using IMU's basic functions
- sonar:
  - Sonar logic class, used to read distance from ground and determine whether or not the foot of the user is grounded
- relay:
  - Relay logic class,  used to turn ON/OFF any relay
- touchScreen:
  - Used to customise the interface and functions of the HMI
- test:
  - Testing class composed of units test for every component
  
## Testing

To ensure the robustness of our system, you can run unit tests on all components by using the Test class.

## License

This project is open source, and you can access the full text of the license [here](link-to-license).

## Acknowledgments

We invite you to explore the [Robotique UdeS website](https://robotiqueudes.ca/) to learn more about our team and discover other intriguing projects we're involved in.

## Authors

The masterminds behind the Exo_Control project are:

- Édouard Moffet
- Dannick Bilodeau
- Feel free to add your name on your first commit!
