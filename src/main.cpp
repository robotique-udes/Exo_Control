#include <Arduino.h>
#include <Wire.h>
#include "BNOHandler.hpp"
#include "CanBusReceiver.hpp"
#include "CubemarsMotorV2.hpp"
#include "CubemarsMotorV3.hpp"
#include "Logic.hpp"
#include "MotorHandler.hpp"
#include "CubemarsMotorV2.hpp"
#include "CubemarsMotorV3.hpp"
#include "hmi/HMI_Comm.hpp"
#include "Config.hpp"


void hmiLoop(void * pvParameters);

static BnoHandler bnoHandler;
static Logic logic;
static CanBusReceiver canBusReceiver;
static CubemarsMotorV2 kneeLeftMotor(exo_config::motors::KNEE_LEFT);
static CubemarsMotorV2 kneeRightMotor(exo_config::motors::KNEE_RIGHT);
static CubemarsMotorV3 hipLeftMotor(exo_config::motors::HIP_LEFT);
static CubemarsMotorV3 hipRightMotor(exo_config::motors::HIP_RIGHT);
static MotorHandler motorHandler({&kneeLeftMotor, &kneeRightMotor, &hipLeftMotor, &hipRightMotor});
static HMI_Comm hmi;
SemaphoreHandle_t motorPowerMutex;


void hmiLoop(void * pvParameters)
{
	hmi.begin();
	hmi.setLogic(&logic);
	hmi.setMotorHandler(&motorHandler);
	while(true)
	{
		hmi.update();
		delay(1000);
	}
}


void setup() {
	Serial.begin(115200);

	Wire.begin();
	delay(500);

	//starts the hmi logic on the core 0
	xTaskCreatePinnedToCore(
		hmiLoop, 
		"Hmi_Task_on_Core0", 
		10000,      
		NULL,       
		1,          
		NULL,       
		0           //Pinned to Core 0
	);


	Serial.println("Starting BnoHandler...");
	const bool hasConnectedBno = bnoHandler.begin();

	if (!hasConnectedBno) {
		Serial.println("No BNO connected.");
	} else {
		Serial.println("BNO setup complete. Reading connected sensors only.");
	}
	pinMode(exo_config::pins::CAN_TERMINAL, OUTPUT);
	digitalWrite(exo_config::pins::CAN_TERMINAL, HIGH);

	if(canBusReceiver.begin(exo_config::pins::CAN_TX, exo_config::pins::CAN_RX, 1000)) {
		Serial.println("CAN bus started!!!");
	} else {
		Serial.println("CAN bus failed!");
	}

	canBusReceiver.subscribe([](const CanFrame& p_message) { kneeLeftMotor.receiveCommand(p_message); },
															 true, kneeLeftMotor.getMotorID());
	canBusReceiver.subscribe([](const CanFrame& p_message) { kneeRightMotor.receiveCommand(p_message); },
															 true, kneeRightMotor.getMotorID());
	canBusReceiver.subscribe([](const CanFrame& p_message) { hipLeftMotor.receiveCommand(p_message); },
															 true, hipLeftMotor.getMotorID() | hipLeftMotor.REPLY_MESSAGE_CODE);
	canBusReceiver.subscribe([](const CanFrame& p_message) { hipRightMotor.receiveCommand(p_message); },
															 true, hipRightMotor.getMotorID() | hipRightMotor.REPLY_MESSAGE_CODE);

	delay(3000);
}

void loop() {

	canBusReceiver.update();

	bnoHandler.requestData();

	if (exo_config::debug::MAIN)
	{
		Serial.println("---- Connected BNO Data ----");
		bnoHandler.printConnectedBNOsData(0, 5);
		Serial.println();
	}

	float angles[exo_config::bnos::AMOUNT] = {0};
	bool grounded[exo_config::bnos::NB_LEG] = {true};
	bnoHandler.getAngle(angles);
	bnoHandler.getGroundedState(grounded);
	
	for (int i = 0; i < exo_config::bnos::AMOUNT; i++)
		Serial.println(angles[i]);

	Serial.println(grounded[0]);
	Serial.println(grounded[1]);

	float torque[exo_config::motors::AMOUNT] = {0};
	logic.calculateTorque(angles, grounded, torque);
	motorHandler.update({torque[0], torque[1], torque[2], torque[3]});

	delay(100);
}