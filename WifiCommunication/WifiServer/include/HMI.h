
#include <LilyGoLib.h> // Hardware-specific library
#include <LV_Helper.h>
#include "WiFiServer.h"

// #include <WifiServer.h>

#define DEFAULT_MASSE 175.0 // in lbs
#define DEFAULT_HEIGHT  66 // in inches
#define DEFAULT_CHARGE_MASSE 0.0 // in lbs

#define INCREMENT_BUTTON_SIZE 50
#define SAE_SI_BUTTON_SIZE 50
#define RESET_BUTTON_HEIGHT 30
#define RESET_BUTTON_WIDTH 70
#define TITLE_SIZE_W 150
#define TITLE_SIZE_H 50
#define LABEL_SIZE_W 100
#define LABEL_SIZE_H 35
#define ROW 75

#define CONNECTION_TIME_LAPSE 800
#define BATTERY_TIME_LAPSE 30000

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

struct page_exo_settings
{
    
    lv_obj_t * tile_2;
    lv_obj_t * label_title;

    lv_obj_t * label_mass;
    lv_obj_t * label_mass_value;
    lv_obj_t * btn_add_mass;
    lv_obj_t * btn_sub_mass;
    lv_obj_t * label_add_mass;
    lv_obj_t * label_sub_mass;

    lv_obj_t * label_charge_mass;
    lv_obj_t * label_charge_mass_value;
    lv_obj_t * btn_add_charge_mass;
    lv_obj_t * btn_sub_charge_mass;
    lv_obj_t * label_add_charge_mass;
    lv_obj_t * label_sub_charge_mass;

    lv_obj_t * label_height;
    lv_obj_t * label_height_value;
    lv_obj_t * btn_add_height;
    lv_obj_t * btn_sub_height;
    lv_obj_t * label_add_height;
    lv_obj_t * label_sub_height;

    lv_obj_t * label_motor_power;
    lv_obj_t * slider_motor_power;

    lv_obj_t * btn_SAE_SI;
    lv_obj_t * label_SAE_SI;

    lv_obj_t * btn_profiles;
    lv_obj_t * btn_reset;
    lv_obj_t * label_reset;
    
};

struct page_menu
{
    lv_obj_t * tile_0;
    lv_obj_t * label_menu;
    lv_obj_t * label_connection;
    lv_obj_t * btn_watch_settings;
};

struct page_info
{
    lv_obj_t * tile_1;
    lv_obj_t * label_exo_battery;
    lv_obj_t * label_watch_battery; 
};

struct page_watch_settings
{
    lv_obj_t * label_brightness;
    lv_obj_t * slider_brightness;
    lv_obj_t * label_sound;
    lv_obj_t * slider_sound;
    lv_obj_t * btn_back;
    lv_obj_t * btn_reset;
};

struct page_profiles
{
    lv_obj_t * label_title;
    lv_obj_t * btn_delete;
    lv_obj_t * btn_back;
    lv_obj_t * label_profile1;
    lv_obj_t * label_profile2;
    lv_obj_t * label_profile3;
    lv_obj_t * label_profile4;
    lv_obj_t * label_profile5;
};

class HMI
{
    private:
        //lv_style_t * style_base;

        page_menu menu;
        page_exo_settings exo_settings;
        page_info info;
        page_watch_settings watch_settings;
        page_profiles profiles;

        lv_obj_t * tile_view;
        lv_obj_t * screen_watch_settings;
        lv_obj_t * screen_profiles;

        float mass = DEFAULT_MASSE;
        float charge_mass = DEFAULT_CHARGE_MASSE;
        float height = DEFAULT_HEIGHT;
        float motor_power = 0.0;
        bool SAE_SI = false; // false for SAE, true for SI
        bool is_connected_exo = false;
        float brightness = 0.0;
        float sound = 0.0;

        static HMI* hmi_instance;

        //WifiServer * wifiserver = WifiServer::GetInstance();

        void setupTileView();
        void setupStyle();
        void setupExoSettings();
        void setupMenu();
        void setupInfo();
        void setupWatchSettings();
        void setupProfiles();

        void updateMassLabel();
        void updateChargeMassLabel();
        void updateHeightLabel();
        void updateMotorPowerLabel();
        void updateBatteryLabel();
        void updateBrightnessLabel();
        void updateSoundLabel();
        void updateConnectionLabel();
        void update_SAE_SI_Label();
        
        void sendSettingsExo();
        
        public:
        HMI();
        void setup();
        void update();
        void addMass();
        void subMass();
        void addChargeMass();
        void subChargeMass();
        void addHeight();
        void subHeight();
        void toggle_SAE_SI();
        void resetExoSettings();
        static HMI* getInstance();
};
