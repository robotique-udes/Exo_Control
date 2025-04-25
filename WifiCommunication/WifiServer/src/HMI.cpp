#include "HMI.h"

HMI::HMI()
{
    // Constructor implementation
    Serial.println("HMI constructor called");
    
}

void HMI::setup()
{
    Serial.println("HMI setup called");
    // Initialize the screens
    tile_view = lv_tileview_create(lv_scr_act());

    setupStyle();
    setupMenu();
    setupWatchSettings();
    setupExoSettings();
    setupInfo();
    setupProfiles();
}

void HMI::setupStyle()
{
    Serial.println("HMI setupStyle called");
    // Initialize the style
    lv_style_init(style_base);
    lv_style_set_radius(style_base, 10);
    lv_style_set_bg_opa(style_base, LV_OPA_COVER);
    lv_style_set_bg_color(style_base, lv_color_white());
}


void HMI::setupMenu()
{
    Serial.println("HMI setupMenu called");
    // Create the menu screen objects
    menu.tile_middle = lv_tileview_add_tile(tile_view, 0, 0, LV_DIR_LEFT);

    menu.label_menu = lv_label_create(menu.tile_middle);
    lv_label_set_text(menu.label_menu, "Exo Control");
    lv_obj_add_style(menu.label_menu, style_base, 0);
    lv_obj_set_size(menu.label_menu, TITLE_SIZE_W, TITLE_SIZE_H);
    
    menu.label_connection = lv_label_create(menu.tile_middle);
    lv_label_set_text(menu.label_connection, "Connecting");
    lv_obj_add_style(menu.label_connection, style_base, 0);
    lv_obj_set_size(menu.label_connection, LABEL_SIZE_W, LABEL_SIZE_H);
    
    // Set the positions of the objects
    lv_obj_align(menu.label_menu, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_align(menu.label_connection, LV_ALIGN_TOP_MID, 0, 70);
}

void HMI::setupWatchSettings()
{
    Serial.println("HMI setupWatchSettings called");
}

void HMI::setupExoSettings()
{
    Serial.println("HMI setupExoSettings called");
    // Create the exo settings screen objects
    exo_settings.tile_left = lv_tileview_add_tile(tile_view, 0, 0, LV_DIR_LEFT);

    exo_settings.label_mass = lv_label_create(exo_settings.tile_left);
    lv_label_set_text(exo_settings.label_mass, "Mass");
    lv_obj_add_style(exo_settings.label_mass, style_base, 0);
    lv_obj_set_size(exo_settings.label_mass, LABEL_SIZE_W, LABEL_SIZE_H);
    
    exo_settings.label_mass_value = lv_label_create(exo_settings.tile_left);
    lv_label_set_text(exo_settings.label_mass_value, "0.0 kg");
    lv_obj_add_style(exo_settings.label_mass_value, style_base, 0);
    lv_obj_set_size(exo_settings.label_mass_value, LABEL_SIZE_W, LABEL_SIZE_H);
    
    exo_settings.btn_add_mass = lv_btn_create(exo_settings.tile_left);
    lv_obj_add_style(menu.label_menu, style_base, 0);
    lv_obj_set_size(menu.label_menu, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    
    exo_settings.label_add_mass = lv_label_create(exo_settings.btn_add_mass);
    lv_label_set_text(exo_settings.label_add_mass, "+");
    lv_obj_center(exo_settings.label_add_mass);
    lv_obj_set_size(exo_settings.label_add_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    
    exo_settings.btn_sub_mass = lv_btn_create(exo_settings.tile_left);
    lv_obj_add_style(menu.label_menu, style_base, 0);
    lv_obj_set_size(menu.label_menu, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    
    exo_settings.label_sub_mass = lv_label_create(exo_settings.btn_sub_mass);
    lv_label_set_text(exo_settings.label_sub_mass, "-");
    lv_obj_center(exo_settings.label_sub_mass);
    lv_obj_set_size(exo_settings.label_sub_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    
    // exo_settings.label_height = lv_label_create(exo_settings.tile_left);
    // lv_label_set_text(menu.label_menu, "Exo Control");
    // lv_obj_add_style(menu.label_menu, style_base, 0);
    // lv_obj_set_pos(menu.label_menu, 10, 75);  
    // lv_obj_set_size(menu.label_menu, 125, BUTTON_SIZE);
    
    // exo_settings.btn_add_height = lv_btn_create(exo_settings.tile_left);
    // lv_label_set_text(menu.label_menu, "Exo Control");
    // lv_obj_add_style(menu.label_menu, style_base, 0);
    // lv_obj_set_pos(menu.label_menu, 10, 75);  
    // lv_obj_set_size(menu.label_menu, 125, BUTTON_SIZE);
    
    // lv_obj_t * label_add_height = lv_label_create(exo_settings.btn_add_height);
    // lv_label_set_text(label_add_height, "+");
    // lv_obj_center(label_add_height);
    // lv_obj_set_size(label_add_height, BUTTON_SIZE, BUTTON_SIZE);
    
    // exo_settings.btn_sub_height = lv_btn_create(exo_settings.tile_left);
    // lv_label_set_text(menu.label_menu, "Exo Control");
    // lv_obj_add_style(menu.label_menu, style_base, 0);
    // lv_obj_set_pos(menu.label_menu, 10, 75);  
    // lv_obj_set_size(menu.label_menu, 125, BUTTON_SIZE);
    
    // lv_obj_t * label_sub_height = lv_label_create(exo_settings.btn_sub_height);
    // lv_label_set_text(label_sub_height, "-");
    // lv_obj_center(label_sub_height);
    // lv_obj_set_size(label_sub_height, BUTTON_SIZE, BUTTON_SIZE);
    
    // exo_settings.label_motor_power = lv_label_create(exo_settings.tile_left);
    // lv_label_set_text(menu.label_menu, "Exo Control");
    // lv_obj_add_style(menu.label_menu, style_base, 0);
    // lv_obj_set_pos(menu.label_menu, 10, 75);
    // lv_obj_set_size(menu.label_menu, 125, BUTTON_SIZE);
    
    // exo_settings.slider_motor_power = lv_slider_create(exo_settings.tile_left);
    // lv_obj_add_style(menu.label_menu, style_base, 0);
    // lv_obj_set_pos(menu.label_menu, 10, 75);
    // lv_obj_set_size(menu.label_menu, 125, BUTTON_SIZE);
    
    // Set positions of objects
    
    lv_obj_set_pos(exo_settings.label_mass, 10, ROW * 1);  
    lv_obj_set_pos(exo_settings.label_mass_value, 10, ROW * 1 + 30);
    lv_obj_set_pos(menu.label_menu, 120, ROW * 1 + 30);  
    lv_obj_set_pos(menu.label_menu, 160, ROW * 1 + 30);  
}

void HMI::setupInfo()
{
    Serial.println("HMI setupInfo called");
    // Create the info screen objects
    info.tile_right = lv_tileview_add_tile(tile_view, 0, 0, LV_DIR_RIGHT);

    info.label_exo_battery = lv_label_create(info.tile_right);
    lv_label_set_text(info.label_exo_battery, "Exo Battery: 100%");
    lv_obj_add_style(info.label_exo_battery, style_base, 0);
    lv_obj_set_size(info.label_exo_battery, LABEL_SIZE_W, LABEL_SIZE_H);

    info.label_watch_battery = lv_label_create(info.tile_right);
    lv_label_set_text(info.label_watch_battery, "Watch Battery: 100%");
    lv_obj_add_style(info.label_watch_battery, style_base, 0);
    lv_obj_set_size(info.label_watch_battery, LABEL_SIZE_W, LABEL_SIZE_H);

    // Set the positions of the objects
    lv_obj_align(info.label_exo_battery, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_align(info.label_watch_battery, LV_ALIGN_TOP_MID, 0, 70);
}

void HMI::setupProfiles()
{
    Serial.println("HMI setupProfiles called");
}
