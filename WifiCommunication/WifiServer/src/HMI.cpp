#include "HMI.h"


HMI::HMI()
{
    // Constructor implementation
    Serial.println("HMI constructor called");
    beginLvglHelper();
    setup();
    
}

void HMI::setup()
{
    Serial.println("HMI setup called");
    // Initialize the screens
    setupTileView();
    setupStyle();
    setupMenu();
    setupWatchSettings();
    setupExoSettings();
    setupInfo();
    setupProfiles();
}

void HMI::setupTileView()
{
    Serial.println("HMI setupTileView called");
    // Initialize the tile view
    tile_view = lv_tileview_create(lv_scr_act());
    lv_obj_set_style_bg_color(tile_view, lv_color_black(), 0);
    menu.tile_0 = lv_tileview_add_tile(tile_view, 0, 0, LV_DIR_RIGHT);
    info.tile_1 = lv_tileview_add_tile(tile_view, 1, 0, (lv_dir_t)(LV_DIR_LEFT | LV_DIR_RIGHT));
    exo_settings.tile_2 = lv_tileview_add_tile(tile_view, 2, 0, LV_DIR_LEFT);
    
}

void HMI::setupStyle()
{
    Serial.println("HMI setupStyle called");
    // Initialize the style
    // lv_style_init(style_base);
    // lv_style_set_radius(style_base, 10);
    // lv_style_set_bg_opa(style_base, LV_OPA_COVER);
    // lv_style_set_bg_color(style_base, lv_color_white());
}


void HMI::setupMenu()
{
    Serial.println("HMI setupMenu called");
    // Create the menu screen objects
    menu.label_menu = lv_label_create(menu.tile_0);
    lv_label_set_text(menu.label_menu, "Exo Control");
    // lv_obj_add_style(menu.label_menu, style_base, 0);
    lv_obj_set_size(menu.label_menu, TITLE_SIZE_W, TITLE_SIZE_H);
    
    menu.label_connection = lv_label_create(menu.tile_0);
    lv_label_set_text(menu.label_connection, "Connecting...");
    // lv_obj_add_style(menu.label_connection, style_base, 0);
    lv_obj_set_size(menu.label_connection, TITLE_SIZE_W, LABEL_SIZE_H);
    
    // Set the positions of the objects
    lv_obj_align(menu.label_menu, LV_ALIGN_TOP_MID, 0, LV_VER_RES * 1 / 4);
    lv_obj_align(menu.label_connection, LV_ALIGN_TOP_MID, 0, LV_VER_RES * 2 / 4);
}

void HMI::setupWatchSettings()
{
    Serial.println("HMI setupWatchSettings called");
}

void HMI::setupExoSettings()
{
    Serial.println("HMI setupExoSettings called");
    // Create the exo settings screen objects
    // ------------------------------------------mass related objects------------------------------------------------
    exo_settings.label_mass = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_mass, "Mass");
    // lv_obj_add_style(exo_settings.label_mass, style_base, 0);
    lv_obj_set_size(exo_settings.label_mass, LABEL_SIZE_W, LABEL_SIZE_H);
    
    exo_settings.label_mass_value = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_mass_value, "0.0 kg");
    // lv_obj_add_style(exo_settings.label_mass_value, style_base, 0);
    lv_obj_set_size(exo_settings.label_mass_value, LABEL_SIZE_W, LABEL_SIZE_H);
    
    exo_settings.btn_add_mass = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_add_mass, style_base, 0);
    lv_obj_set_size(exo_settings.btn_add_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_add_mass = lv_label_create(exo_settings.btn_add_mass);
    lv_label_set_text(exo_settings.label_add_mass, "+");
    lv_obj_center(exo_settings.label_add_mass);
    
    exo_settings.btn_sub_mass = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_sub_mass, style_base, 0);
    lv_obj_set_size(exo_settings.btn_sub_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_sub_mass = lv_label_create(exo_settings.btn_sub_mass);
    lv_label_set_text(exo_settings.label_sub_mass, "-");
    lv_obj_center(exo_settings.label_sub_mass);

    // ------------------------------------------charge mass related objects------------------------------------------------
    exo_settings.label_charge_mass = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_charge_mass, "Charge Mass");
    // lv_obj_add_style(exo_settings.label_charge_mass, style_base, 0);
    lv_obj_set_size(exo_settings.label_charge_mass, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.label_charge_mass_value = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_charge_mass_value, "0.0 kg");
    // lv_obj_add_style(exo_settings.label_charge_mass_value, style_base, 0);
    lv_obj_set_size(exo_settings.label_charge_mass_value, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.btn_add_charge_mass = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_add_charge_mass, style_base, 0);
    lv_obj_set_size(exo_settings.btn_add_charge_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_add_charge_mass = lv_label_create(exo_settings.btn_add_charge_mass);
    lv_label_set_text(exo_settings.label_add_charge_mass, "+");
    lv_obj_center(exo_settings.label_add_charge_mass);

    exo_settings.btn_sub_charge_mass = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_sub_charge_mass, style_base, 0);
    lv_obj_set_size(exo_settings.btn_sub_charge_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_sub_charge_mass = lv_label_create(exo_settings.btn_sub_charge_mass);
    lv_label_set_text(exo_settings.label_sub_charge_mass, "-");
    lv_obj_center(exo_settings.label_sub_charge_mass);

    // ------------------------------------------height related objects------------------------------------------------
    exo_settings.label_height = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_height, "Height");
    // lv_obj_add_style(exo_settings.label_height, style_base, 0);
    lv_obj_set_size(exo_settings.label_height, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.label_height_value = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_height_value, "0.0 m");
    // lv_obj_add_style(exo_settings.label_height_value, style_base, 0);
    lv_obj_set_size(exo_settings.label_height_value, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.btn_add_height = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_add_height, style_base, 0);
    lv_obj_set_size(exo_settings.btn_add_height, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_add_height = lv_label_create(exo_settings.btn_add_height);
    lv_label_set_text(exo_settings.label_add_height, "+");
    lv_obj_center(exo_settings.label_add_height);

    exo_settings.btn_sub_height = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_sub_height, style_base, 0);
    lv_obj_set_size(exo_settings.btn_sub_height, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_sub_height = lv_label_create(exo_settings.btn_sub_height);
    lv_label_set_text(exo_settings.label_sub_height, "-");
    lv_obj_center(exo_settings.label_sub_height);

    // ------------------------------------------motor power related objects------------------------------------------------
    exo_settings.label_motor_power = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_motor_power, "Motor Power");
    // lv_obj_add_style(exo_settings.label_motor_power, style_base, 0);
    lv_obj_set_size(exo_settings.label_motor_power, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.slider_motor_power = lv_slider_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.slider_motor_power, style_base, 0);
    lv_obj_set_size(exo_settings.slider_motor_power, LABEL_SIZE_W, LABEL_SIZE_H);
    lv_slider_set_range(exo_settings.slider_motor_power, 0, 200);
    lv_slider_set_value(exo_settings.slider_motor_power, 100, LV_ANIM_OFF);
    
    // Set positions of objects
    // mass related objects
    lv_obj_set_pos(exo_settings.label_mass, 10, ROW * 1);  
    lv_obj_align_to(exo_settings.label_mass_value, exo_settings.label_mass, LV_ALIGN_CENTER, 0, 30);
    lv_obj_align_to(exo_settings.btn_add_mass, exo_settings.label_mass, LV_ALIGN_CENTER, 90, 0);
    lv_obj_align_to(exo_settings.btn_sub_mass, exo_settings.label_mass, LV_ALIGN_CENTER, 160, 0);

    // charge mass related objects
    lv_obj_set_pos(exo_settings.label_charge_mass, 10, ROW * 2);
    lv_obj_align_to(exo_settings.label_charge_mass_value, exo_settings.label_charge_mass, LV_ALIGN_CENTER, 0, 30);
    lv_obj_align_to(exo_settings.btn_add_charge_mass, exo_settings.label_charge_mass, LV_ALIGN_CENTER, 90, 0);
    lv_obj_align_to(exo_settings.btn_sub_charge_mass, exo_settings.label_charge_mass, LV_ALIGN_CENTER, 160, 0);

    // height related objects
    lv_obj_set_pos(exo_settings.label_height, 10, ROW * 3);
    lv_obj_align_to(exo_settings.label_height_value, exo_settings.label_height, LV_ALIGN_CENTER, 0, 30);
    lv_obj_align_to(exo_settings.btn_add_height, exo_settings.label_height, LV_ALIGN_CENTER, 90, 0);
    lv_obj_align_to(exo_settings.btn_sub_height, exo_settings.label_height, LV_ALIGN_CENTER, 160, 0);

    // motor power related objects
    lv_obj_set_pos(exo_settings.label_motor_power, 10, ROW * 4);
    lv_obj_align_to(exo_settings.slider_motor_power, exo_settings.label_motor_power, LV_ALIGN_CENTER, 0, 30);
    
}

void HMI::setupInfo()
{
    Serial.println("HMI setupInfo called");
    // Create the info screen objects
    info.label_exo_battery = lv_label_create(info.tile_1);
    lv_label_set_text(info.label_exo_battery, "Exo Battery: 100%");
    // lv_obj_add_style(info.label_exo_battery, style_base, 0);
    lv_obj_set_size(info.label_exo_battery, LABEL_SIZE_W, LABEL_SIZE_H);

    info.label_watch_battery = lv_label_create(info.tile_1);
    lv_label_set_text(info.label_watch_battery, "Watch Battery: 100%");
    // lv_obj_add_style(info.label_watch_battery, style_base, 0);
    lv_obj_set_size(info.label_watch_battery, LABEL_SIZE_W, LABEL_SIZE_H);

    // Set the positions of the objects
    lv_obj_align(info.label_exo_battery, LV_ALIGN_TOP_MID, 0, LV_VER_RES * 1 / 4);
    lv_obj_align(info.label_watch_battery, LV_ALIGN_TOP_MID, 0, LV_VER_RES * 2 / 4);
}

void HMI::setupProfiles()
{
    Serial.println("HMI setupProfiles called");
}

void HMI::update()
{
    // Call this function in the main loop to update the HMI. The HMI will handle the display by itself.
    Serial.println("HMI update called");
    // time count
    static unsigned long previousMillisConnection = 0; 
    unsigned long currentMillis = millis();

    // update the connection status
    if (currentMillis - previousMillisConnection >= CONNECTION_TIME_LAPSE)
    {
        previousMillisConnection = currentMillis;
        // Get the exoskeleton connection to the watch status
        if (true)
        {
            Serial.println("HMI update called: connected to exoskeleton");
            const char * text = lv_label_get_text(menu.label_connection);
            // update the connection label
            if (strcmp(text, "Connecting"))
            {
                lv_label_set_text(menu.label_connection, "Connecting.");
            }
            else if (strcmp(text, "Connecting."))
            {
                lv_label_set_text(menu.label_connection, "Connecting..");
            }
            else if (strcmp(text, "Connecting.."))
            {
                lv_label_set_text(menu.label_connection, "Connecting...");
            }
            else if (strcmp(text, "Connecting..."))
            {
                lv_label_set_text(menu.label_connection, "Connecting");
            }
            else
            {
                lv_label_set_text(menu.label_connection, "Connected");
            }

        }
        else 
        {
            lv_label_set_text(menu.label_connection, "Connected");
        }
    }

    lv_task_handler();
}