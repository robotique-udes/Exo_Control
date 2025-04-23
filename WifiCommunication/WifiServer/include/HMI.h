
#include <LilyGoLib.h> // Hardware-specific library
#include <LV_Helper.h>

#define DEFAULT_MASSE "170"
#define DEFAULT_HEIGHT "150"

#define BUTTON_SIZE 35
#define ROW1 75
#define ROW2 150

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

struct page_exo_settings
{
    lv_obj_t * label_mass;
    lv_obj_t * btn_add_mass;
    lv_obj_t * btn_sub_mass;

    lv_obj_t * label_height;
    lv_obj_t * btn_add_height;
    lv_obj_t * btn_sub_height;

    lv_obj_t * label_motor_power;
    lv_obj_t * slider_motor_power;

    lv_obj_t * btn_bool_SAE_SI;

    lv_obj_t * btn_profiles;
    lv_obj_t * btn_reset;
    
};

struct page_menu
{
    lv_obj_t * label_menu;
    lv_obj_t * label_connection;
    lv_obj_t * btn_watch_settings;
};

struct page_info
{
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
        page_exo_settings exo_settings;
        page_menu menu;
        page_info info;
        page_watch_settings watch_settings;
        page_profiles profiles;

        lv_obj_t * screen_exo_settings;
        lv_obj_t * screen_menu;
        lv_obj_t * screen_info;
        lv_obj_t * screen_watch_settings;
        lv_obj_t * screen_profiles;

        
    
    public:
        HMI();
        void setup();
        void setupExoSettings();
        void setupMenu();
        void setupInfo();
        void setupWatchSettings();
        void setupProfiles();
        
};
