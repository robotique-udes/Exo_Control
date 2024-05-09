
// PINS SONAR
#define TRIG_PIN_GAUCHE 15
#define ECHO_PIN_GAUCHE 39
#define TRIG_PIN_DROIT 23
#define ECHO_PIN_DROIT 35
// constant SONAR
#define UNCERTAINTY 0.1
#define LOW 0x0
#define HIGH 0x1

// ------------ MOTOR -------------
#define K_RESSORT 0.25
#define DIST_CM 0.245
#define MASSE 0.366
#define PULSE_PAR_TOUR 300
#define TORQUE2CURRENT 0.6 // 6.470
#define KP 2.5
#define KI 0
#define KD 0
#define HIGH_TORQUE 100

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

#define PIN_MD_GEN_GAU_EN 3  // Pin EN pour le moteur du genou gauche
#define PIN_MD_GEN_DRO_EN 7  // Pin EN pour le moteur du genou droit
#define PIN_MD_HAN_GAU_EN 12 // Pin EN pour le moteur de la hanche gauche
#define PIN_MD_HAN_DRO_EN 15 // Pin EN pour le moteur de la hanche droite

#define PIN_MD_HAN_GAU_CT 3 // Pin de current sensing pour le moteur de la hanche gauche
#define PIN_MD_HAN_DRO_CT 4 // Pin de current sensing pour le moteur de la hanche droite
#define PIN_MD_GEN_GAU_CT 5 // Pin de current sensing pour le moteur du genou gauche
#define PIN_MD_GEN_DRO_CT 6 // Pin de current sensing pour le moteur du genou droit

// LONGUEURS
#define LH 0.5  // longueur haut du corps
#define LF 0.4  // longueur femur
#define LT 0.4  // longueur tibia
#define G 9.81  // gravité
#define MH 25.0 // Masse haut du corps/2
#define MF 3.0  // Masse femur
#define MT 3.0  // Masse tibia

#define ClockWise 1
#define CounterClockWise -1

#define MOTEUR_GENOU_GAUCHE 0
#define MOTEUR_GENOU_DROIT 1
#define MOTEUR_HANCHE_GAUCHE 2
#define MOTEUR_HANCHE_DROITE 3
#define SONAR_GAUCHE 0
#define SONAR_DROIT 1

#define RELAIS_GENOU_GAUCHE 21
#define RELAIS_GENOU_DROIT 22
#define RELAIS_HANCHE_GAUCHE 13
#define RELAIS_HANCHE_DROITE 4

#define ON 1
#define OFF 0

//------IMU--------
#define I2C_SDA1 26
#define I2C_SCL1 27
#define I2C_SDA2 33
#define I2C_SCL2 32
#define I2C_SDA3 25
#define I2C_SCL3 14
/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (800)

//------ TOUCH SCREEN ------
#define RX_HMI 16
#define TX_HMI 17
