

//**********PAS LES BONNES PINS************
#define SDA_PIN_GAUCHE 15
#define SCL_PIN_GAUCHE 39
#define SDA_PIN_DROIT 23
#define SCL_PIN_DROIT 35

// ------------ MOTOR -------------
#define K_RESSORT 0.25
#define DIST_CM 0.245
#define MASSE 0.366
#define PULSE_PAR_TOUR 300
#define TORQUE2CURRENT 0.6 // 6.470
#define KP 2.5
#define KI 0
#define KD 0
#define HIGHEST_KNEE_TORQUE 38.0f
#define HIGHEST_HIP_TORQUE 48.0f
#define MOTOR_KM 1.27
#define MOTOR_W_PER_NM_HIP 30.72
#define MOTOR_W_PER_NM_KNEE 52.57
#define MILLIS_TO_HOUR 0.001 / 3600.0
#define P_MAX 12.5f
#define P_MIN -12.5f
#define V_MIN -50.0f
#define V_MAX 50.0f 
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -65.0f  
#define T_MAX 65.0f 

#define MOTEUR_GENOU_GAUCHE 0
#define MOTEUR_GENOU_DROIT 1
#define MOTEUR_HANCHE_GAUCHE 2
#define MOTEUR_HANCHE_DROITE 3

#define NB_MOTORS 4         //nombre de moteurs sur l'exo
#define TEMP_THRESHOLD 75   // la temperature max que les moteur peuvent atteindre
#define SHUT_DOWN_TIME 10   // the time that is takes to shut down motors when overheating 
#define CAN_RX 5      
#define CAN_TX 6      

// LONGUEURS
#define G 9.81  // gravité
#define M_BACK 25.0 // Masse du haut du corps
#define M_THIGH 3.0 // Masse de la cuisse
#define M_TIBIA 3.0 // Masse du tibia
#define L_BACK 0.5 // Longueur du haut du corps
#define L_THIGH 0.4 // Longueur de la cuisse
#define L_TIBIA 0.4 // Longueur du tibia

#define ClockWise 1
#define CounterClockWise -1

#define MOTEUR_GENOU_GAUCHE 0
#define MOTEUR_GENOU_DROIT 1
#define MOTEUR_HANCHE_GAUCHE 2
#define MOTEUR_HANCHE_DROITE 3


#define ON 1
#define OFF 0

//------ I2C --------
#define I2C_SDA1 26
#define I2C_SCL1 27
#define I2C_SDA2 33
#define I2C_SCL2 32
#define I2C_SDA3 25
#define I2C_SCL3 14

#define IMU_DEBUG true
#define MAIN_I2C_SDA 21
#define MAIN_I2C_SCL 22
#define LEFT_MOUSTACHE_MUX_CHANNEL 1
#define RIGHT_MOUSTACHE_MUX_CHANNEL 2 
#define I2C_CLOCK   400000L 

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (800)

//------ TOUCH SCREEN ------
#define RX_HMI 16
#define TX_HMI 17

//------ SETTINGS -------
#define WALK_MODE 0
#define SQUAT_MODE 1
#define FROM_IMU 0
#define FROM_ENCODER 1
#define MOTOR_INCREASE 5
#define PULSES_PER_REVOLUTION 925

//------ MAX JOINT ROTATION ANGLE -------
#define MAX_ANGLE_HIP 110
#define MIN_ANGLE_HIP 0
#define MAX_ANGLE_KNEE 90
#define MIN_ANGLE_KNEE 0

//------ BNO
#define BUFFER_SIZE 8
#define ACCEL_THRESHOLD 150
#define ACCEL_BUFFER_THRESHOLD 0.5
