

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

#define MOTEUR_GENOU_GAUCHE 0
#define MOTEUR_GENOU_DROIT 1
#define MOTEUR_HANCHE_GAUCHE 2
#define MOTEUR_HANCHE_DROITE 3

#define PIN_MD_HAN_DRO_INB 0 // Pin INB pour le moteur de la hanche droite
#define PIN_MD_HAN_DRO_INA 1 // Pin INA pour le moteur de la hanche droite
#define PIN_MD_HAN_GAU_INB 2 // Pin INB pour le moteur de la hanche gauche
#define PIN_MD_HAN_GAU_INA 3 // Pin INA pour le moteur de la hanche gauche
#define PIN_MD_GEN_DRO_INB 4 // Pin INB pour le moteur du genou droit
#define PIN_MD_GEN_DRO_INA 5 // Pin INA pour le moteur du genou droit
#define PIN_MD_GEN_GAU_INB 6 // Pin INB pour le moteur du genou gauche
#define PIN_MD_GEN_GAU_INA 7 // Pin INA pour le moteur du genou gauche

#define PIN_MD_GEN_GAU_EN 0  // Pin EN pour le moteur du genou gauche
#define PIN_MD_GEN_DRO_EN 3  // Pin EN pour le moteur du genou droit
#define PIN_MD_HAN_GAU_EN 7 // Pin EN pour le moteur de la hanche gauche
#define PIN_MD_HAN_DRO_EN 10 // Pin EN pour le moteur de la hanche droite

#define PIN_MD_HAN_GAU_CT 36 // Pin de current sensing pour le moteur de la hanche gauche
#define PIN_MD_HAN_DRO_CT 39 // Pin de current sensing pour le moteur de la hanche droite
#define PIN_MD_GEN_GAU_CT 34 // Pin de current sensing pour le moteur du genou gauche
#define PIN_MD_GEN_DRO_CT 35 // Pin de current sensing pour le moteur du genou droit

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

#define RELAIS_GENOU_GAUCHE 13
#define RELAIS_GENOU_DROIT 14
#define RELAIS_HANCHE_GAUCHE 15
#define RELAIS_HANCHE_DROITE 16

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

//------ PROXIM ------
#define PROXIM_REFRESH_RATE 4
#define GROUND_DISTANCE_RANGE 40


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
