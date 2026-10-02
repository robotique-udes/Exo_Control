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
#include "macros/printMacro.hpp"

static const bool DEBUG_PRINT = true; // true if you want to display prints in this file

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


void setup() 
{
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


	PRINTLN("Starting BnoHandler...");
	const bool hasConnectedBno = bnoHandler.begin();

	if (!hasConnectedBno) {
		PRINTLN("No BNO connected.");
	} else {
		PRINTLN("BNO setup complete. Reading connected sensors only.");
	}
	pinMode(exo_config::pins::CAN_TERMINAL, OUTPUT);
	digitalWrite(exo_config::pins::CAN_TERMINAL, HIGH);

	if(canBusReceiver.begin(exo_config::pins::CAN_TX, exo_config::pins::CAN_RX, 1000)) {
		PRINTLN("CAN bus started!!!");
	} else {
		PRINTLN("CAN bus failed!");
	}

	canBusReceiver.addObserver(&kneeLeftMotor);
	canBusReceiver.addObserver(&kneeRightMotor);
	canBusReceiver.addObserver(&hipLeftMotor);
	canBusReceiver.addObserver(&hipRightMotor);

	delay(3000);
}

void loop() {

	canBusReceiver.update();

	bnoHandler.requestData();
	PRINTLN("---- Connected BNO Data ----");
	bnoHandler.printConnectedBNOsData(0, 5);
	PRINTLN("");

	float angles[exo_config::bnos::AMOUNT] = {0};
	bool grounded[exo_config::bnos::NB_LEG] = {true};
	bnoHandler.getAngle(angles);
	bnoHandler.getGroundedState(grounded);
	
	for (int i = 0; i < exo_config::bnos::AMOUNT; i++)
		PRINTLN(angles[i]);

	PRINTLN(grounded[0]);
	PRINTLN(grounded[1]);

	float torque[exo_config::motors::AMOUNT] = {0};
	logic.calculateTorque(angles, grounded, torque);
	motorHandler.update({torque[0], torque[1], torque[2], torque[3]});

	delay(100);
}