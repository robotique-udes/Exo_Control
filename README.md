# Core

## Table of Contents

- [Core](#core)
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
- **Motor**: Servocity 638326

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
- dataCore:
  - Class holding every middleman variable for every other classes.
  - Mostly composed of setter and getter
- bnoHandler:
  - This class orchestrates the calls to and from each BNO.
- multiplex:
  - Wrapper class to facilitate the use of i2c devices going through the multiplexer
- BNO_085:
  - Logic class to interact with the Adafruit BNO_085. Stores data in the BnoStruct
  format for easy access, accessible with a getter.
- proxiHandler:
  - This class orchestrates the calls to and from each proximity sensor.
- proximSensor:
  - Proxim logic class, used to read distance from ground and determine whether or not the foot of the user is grounded
- logic:
  - This class handles mathematic operations. 
  - It takes the angles of the legs and compute the required pwm out of it.
- motor:
  - Mainly send commands to directly to the motors.
- quadratureEncoder:
  - Alternative way to get the leg angles from the motor encoders.
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

- Édouard Moffet, robot 67
- Dannick Bilodeau,  info 68
- Simon Trudeau,  info 68
- Jorand Gagnon,  elec 69
