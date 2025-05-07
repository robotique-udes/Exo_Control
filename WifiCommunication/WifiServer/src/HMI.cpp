#include "HMI.h"

// Declare event handlers
void addMassEvent(lv_event_t *e);
void subMassEvent(lv_event_t *e);
void addChargeMassEvent(lv_event_t *e);
void subChargeMassEvent(lv_event_t *e);
void addHeightEvent(lv_event_t *e);
void subHeightEvent(lv_event_t *e);
void toggle_SAE_SI_Event(lv_event_t *e);
void resetEvent(lv_event_t *e);


// HMI class implementation

HMI* HMI::hmi_instance = nullptr;
HMI* HMI::getInstance()
{
    if (hmi_instance == nullptr)
    {
        hmi_instance = new HMI();
    }
    return hmi_instance;
}

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
    lv_obj_set_style_text_color(menu.label_menu, lv_color_white(), 0);
    // lv_obj_add_style(menu.label_menu, style_base, 0);
    lv_obj_set_size(menu.label_menu, TITLE_SIZE_W, TITLE_SIZE_H);
    
    menu.label_connection = lv_label_create(menu.tile_0);
    lv_label_set_text(menu.label_connection, "Connecting...");
    lv_obj_set_style_text_color(menu.label_connection, lv_color_white(), 0);
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

    // title, sae_si toggle button and reset button
    exo_settings.label_title = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_title, "Exo Settings");
    lv_obj_set_style_text_color(exo_settings.label_title, lv_color_white(), 0);
    // lv_obj_add_style(exo_settings.label_mass, style_base, 0);
    lv_obj_set_size(exo_settings.label_mass, TITLE_SIZE_W, TITLE_SIZE_H);

    exo_settings.btn_SAE_SI = lv_btn_create(exo_settings.tile_2);
    lv_obj_set_size(exo_settings.btn_SAE_SI, SAE_SI_BUTTON_SIZE, SAE_SI_BUTTON_SIZE);
    exo_settings.label_SAE_SI = lv_label_create(exo_settings.btn_SAE_SI);
    update_SAE_SI_Label();
    lv_obj_center(exo_settings.label_SAE_SI);
    lv_obj_add_event_cb(exo_settings.btn_SAE_SI, toggle_SAE_SI_Event, LV_EVENT_ALL, NULL);

    exo_settings.btn_reset = lv_btn_create(exo_settings.tile_2);
    lv_obj_set_size(exo_settings.btn_reset, RESET_BUTTON_WIDTH, RESET_BUTTON_HEIGHT);
    exo_settings.label_reset = lv_label_create(exo_settings.btn_reset);
    lv_label_set_text(exo_settings.label_reset, "Reset");
    lv_obj_center(exo_settings.label_reset);
    lv_obj_add_event_cb(exo_settings.btn_reset, resetEvent, LV_EVENT_ALL, NULL);

    // ------------------------------------------mass related objects------------------------------------------------
    exo_settings.label_mass = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_mass, "Mass");
    lv_obj_set_style_text_color(exo_settings.label_mass, lv_color_white(), 0);
    // lv_obj_add_style(exo_settings.label_mass, style_base, 0);
    lv_obj_set_size(exo_settings.label_mass, LABEL_SIZE_W, LABEL_SIZE_H);
    
    exo_settings.label_mass_value = lv_label_create(exo_settings.tile_2);
    updateMassLabel();
    lv_obj_set_style_text_color(exo_settings.label_mass_value, lv_color_white(), 0);
    // lv_obj_add_style(exo_settings.label_mass_value, style_base, 0);
    lv_obj_set_size(exo_settings.label_mass_value, LABEL_SIZE_W, LABEL_SIZE_H);
    
    exo_settings.btn_add_mass = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_add_mass, style_base, 0);
    lv_obj_set_size(exo_settings.btn_add_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_add_mass = lv_label_create(exo_settings.btn_add_mass);
    lv_label_set_text(exo_settings.label_add_mass, "+");
    lv_obj_center(exo_settings.label_add_mass);
    lv_obj_add_event_cb(exo_settings.btn_add_mass, addMassEvent, LV_EVENT_ALL, NULL); 
    
    exo_settings.btn_sub_mass = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_sub_mass, style_base, 0);
    lv_obj_set_size(exo_settings.btn_sub_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_sub_mass = lv_label_create(exo_settings.btn_sub_mass);
    lv_label_set_text(exo_settings.label_sub_mass, "-");
    lv_obj_center(exo_settings.label_sub_mass);
    lv_obj_add_event_cb(exo_settings.btn_sub_mass, subMassEvent, LV_EVENT_ALL, NULL);

    // ------------------------------------------charge mass related objects------------------------------------------------
    exo_settings.label_charge_mass = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_charge_mass, "Charge Mass");
    lv_obj_set_style_text_color(exo_settings.label_charge_mass, lv_color_white(), 0);
    // lv_obj_add_style(exo_settings.label_charge_mass, style_base, 0);
    lv_obj_set_size(exo_settings.label_charge_mass, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.label_charge_mass_value = lv_label_create(exo_settings.tile_2);
    updateChargeMassLabel();
    lv_obj_set_style_text_color(exo_settings.label_charge_mass_value, lv_color_white(), 0);
    // lv_obj_add_style(exo_settings.label_charge_mass_value, style_base, 0);
    lv_obj_set_size(exo_settings.label_charge_mass_value, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.btn_add_charge_mass = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_add_charge_mass, style_base, 0);
    lv_obj_set_size(exo_settings.btn_add_charge_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_add_charge_mass = lv_label_create(exo_settings.btn_add_charge_mass);
    lv_label_set_text(exo_settings.label_add_charge_mass, "+");
    lv_obj_center(exo_settings.label_add_charge_mass);
    lv_obj_add_event_cb(exo_settings.btn_add_charge_mass, addChargeMassEvent, LV_EVENT_ALL, NULL);

    exo_settings.btn_sub_charge_mass = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_sub_charge_mass, style_base, 0);
    lv_obj_set_size(exo_settings.btn_sub_charge_mass, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_sub_charge_mass = lv_label_create(exo_settings.btn_sub_charge_mass);
    lv_label_set_text(exo_settings.label_sub_charge_mass, "-");
    lv_obj_center(exo_settings.label_sub_charge_mass);
    lv_obj_add_event_cb(exo_settings.btn_sub_charge_mass, subChargeMassEvent, LV_EVENT_ALL, NULL);

    // ------------------------------------------height related objects------------------------------------------------
    exo_settings.label_height = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_height, "Height");
    lv_obj_set_style_text_color(exo_settings.label_height, lv_color_white(), 0);
    // lv_obj_add_style(exo_settings.label_height, style_base, 0);
    lv_obj_set_size(exo_settings.label_height, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.label_height_value = lv_label_create(exo_settings.tile_2);
    updateHeightLabel();
    lv_obj_set_style_text_color(exo_settings.label_height_value, lv_color_white(), 0);
    // lv_obj_add_style(exo_settings.label_height_value, style_base, 0);
    lv_obj_set_size(exo_settings.label_height_value, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.btn_add_height = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_add_height, style_base, 0);
    lv_obj_set_size(exo_settings.btn_add_height, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_add_height = lv_label_create(exo_settings.btn_add_height);
    lv_label_set_text(exo_settings.label_add_height, "+");
    lv_obj_center(exo_settings.label_add_height);
    lv_obj_add_event_cb(exo_settings.btn_add_height, addHeightEvent, LV_EVENT_ALL, NULL);

    exo_settings.btn_sub_height = lv_btn_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.btn_sub_height, style_base, 0);
    lv_obj_set_size(exo_settings.btn_sub_height, INCREMENT_BUTTON_SIZE, INCREMENT_BUTTON_SIZE);
    exo_settings.label_sub_height = lv_label_create(exo_settings.btn_sub_height);
    lv_label_set_text(exo_settings.label_sub_height, "-");
    lv_obj_center(exo_settings.label_sub_height);
    lv_obj_add_event_cb(exo_settings.btn_sub_height, subHeightEvent, LV_EVENT_ALL, NULL);

    // ------------------------------------------motor power related objects------------------------------------------------
    exo_settings.label_motor_power = lv_label_create(exo_settings.tile_2);
    lv_label_set_text(exo_settings.label_motor_power, "Motor Power");
    lv_obj_set_style_text_color(exo_settings.label_motor_power, lv_color_white(), 0);
    // lv_obj_add_style(exo_settings.label_motor_power, style_base, 0);
    lv_obj_set_size(exo_settings.label_motor_power, LABEL_SIZE_W, LABEL_SIZE_H);

    exo_settings.slider_motor_power = lv_slider_create(exo_settings.tile_2);
    // lv_obj_add_style(exo_settings.slider_motor_power, style_base, 0);
    lv_obj_set_size(exo_settings.slider_motor_power, LABEL_SIZE_W, LABEL_SIZE_H);
    lv_slider_set_range(exo_settings.slider_motor_power, 0, 200);
    lv_slider_set_value(exo_settings.slider_motor_power, 100, LV_ANIM_OFF);
    
    // Set positions of objects
    // title
    lv_obj_set_pos(exo_settings.label_title, 10, 0);

    // SAE SI button
    lv_obj_align_to(exo_settings.btn_SAE_SI, exo_settings.label_title, LV_ALIGN_CENTER, 200, 0);

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

    // reset button
    lv_obj_set_pos(exo_settings.btn_reset, 10, ROW * 5);
    
}

void HMI::setupInfo()
{
    Serial.println("HMI setupInfo called");
    // Create the info screen objects
    info.label_exo_battery = lv_label_create(info.tile_1);
    lv_label_set_text(info.label_exo_battery, "Exo Battery: 100%");
    lv_obj_set_style_text_color(info.label_exo_battery, lv_color_white(), 0);
    // lv_obj_add_style(info.label_exo_battery, style_base, 0);
    lv_obj_set_size(info.label_exo_battery, LABEL_SIZE_W, LABEL_SIZE_H);

    info.label_watch_battery = lv_label_create(info.tile_1);
    lv_label_set_text(info.label_watch_battery, "Watch Battery: 100%");
    lv_obj_set_style_text_color(info.label_watch_battery, lv_color_white(), 0);
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

void HMI::updateBatteryLabel()
{
    // Get the current battery value of the watch
    int battery_watch = watch.getBatteryPercent();
    
    // Update the battery labels with the current values
    char buffer[30];
    sprintf(buffer, "Watch Battery: %d %%", battery_watch);
    lv_label_set_text(info.label_watch_battery, buffer);
}

void HMI::addMass()
{
    // change the mass value
    mass = round(mass + 1);
    
    // update the mass label
    updateMassLabel();

    // Send the new mass value to the exoskeleton
    sendSettingsExo();
}

void HMI::subMass()
{
    // change the mass value
    mass = round(mass - 1);
    
    // update the mass label
    updateMassLabel();

    // Send the new mass value to the exoskeleton
    sendSettingsExo();
}

void HMI::updateMassLabel()
{
    // Update the mass label with the current value
    char buffer[30];
    if (SAE_SI)
    {
        sprintf(buffer, "%.1f kg", mass);
    }
    else
    {
        sprintf(buffer, "%.1f lbs", mass);
    }
    lv_label_set_text(exo_settings.label_mass_value, buffer);
}

void HMI::addChargeMass()
{
    // change the charge mass value
    charge_mass = round(charge_mass + 1);
    
    // update the charge mass label
    updateChargeMassLabel();

    // Send the new charge mass value to the exoskeleton
    sendSettingsExo();
}

void HMI::subChargeMass()
{
    // change the charge mass value
    charge_mass = round(charge_mass - 1);
    
    // update the charge mass label
    updateChargeMassLabel();

    // Send the new charge mass value to the exoskeleton
    sendSettingsExo(); 
}

void HMI::updateChargeMassLabel()
{
    // Update the charge mass label with the current value
    char buffer[30];
    if (SAE_SI)
    {
        sprintf(buffer, "%.1f kg", charge_mass);
    }
    else
    {
        sprintf(buffer, "%.1f lbs", charge_mass);
    }
    lv_label_set_text(exo_settings.label_charge_mass_value, buffer);
}

void HMI::addHeight()
{
    // change the height value
    height = round(height + 1);
    
    // update the height label
    updateHeightLabel();

    // Send the new height value to the exoskeleton
    sendSettingsExo();
}

void HMI::subHeight()
{
    // change the height value
    height = round(height - 1);
    
    // update the height label
    updateHeightLabel();

    // Send the new height value to the exoskeleton
    sendSettingsExo();
}

void HMI::updateHeightLabel()
{
    // Update the height label with the current value
    char buffer[30];
    if (SAE_SI)
    {
        sprintf(buffer, "%.1f m", height);
    }
    else
    {
        int feet = (int)(height / 12);
        int inches = (int)(height - feet * 12);
        sprintf(buffer, "%d'%d\"", feet, inches); 
    }
    lv_label_set_text(exo_settings.label_height_value, buffer);
}

void HMI::toggle_SAE_SI()
{
    // change the SAE_SI value
    SAE_SI = !SAE_SI;
    
    // update the SAE_SI label
    if (SAE_SI)
    {
        // change values to SI units
        mass *= 0.453592; // convert lbs to kg
        charge_mass *= 0.453592; // convert lbs to kg
        height *= 0.0254; // convert inches to m
    }
    else
    {
        // change values to SAE units
        mass *= 2.20462; // convert kg to lbs
        charge_mass *= 2.20462; // convert kg to lbs
        height *= 39.3701; // convert m to inches
    }

    // update the labels
    update_SAE_SI_Label();
    updateMassLabel();
    updateChargeMassLabel();
    updateHeightLabel();

}

void HMI::update_SAE_SI_Label()
{
    // Update the SAE_SI label with the current value
    if (SAE_SI)
    {
        lv_label_set_text(exo_settings.label_SAE_SI, "SI");
    }
    else
    {
        lv_label_set_text(exo_settings.label_SAE_SI, "SAE");
    }
}

void HMI::resetExoSettings()
{
    // reset the exoskeleton settings to default values
    mass = DEFAULT_MASSE;
    charge_mass = DEFAULT_CHARGE_MASSE;
    height = DEFAULT_HEIGHT;
    SAE_SI = false;

    // update the labels
    updateMassLabel();
    updateChargeMassLabel();
    updateHeightLabel();

    // send new data to the exo
    sendSettingsExo();
}

void HMI::sendSettingsExo()
{
    // Send the new height value to the exoskeleton
    // WifiServer* wifi = WifiServer::GetInstance();

    // MessageBuilder message;
    // message.add(EnumInformations::MASSE, mass);
    // message.add(EnumInformations::CHARGE_MASSE, charge_mass);
    // message.add(EnumInformations::HEIGHT, height);
    // int length = message.buildMessage();
    // Serial.print("Message length: ");
    // Serial.println(length);

    // wifi->SendData(message.getMessage(), length, EnumIPType::EXOSKELETON);
}

void HMI::update()
{
    // Call this function in the main loop to update the HMI. The HMI will handle the display by itself.
    //Serial.println("HMI update called");
    // time count
    static unsigned long previousMillisConnection = 0; 
    static unsigned long previousMillisBattery = BATTERY_TIME_LAPSE;
    unsigned long currentMillis = millis();
    //Serial.println(currentMillis);
    WifiServer * wifiserver = WifiServer::GetInstance();

    // update the connection status
    if (currentMillis - previousMillisConnection >= CONNECTION_TIME_LAPSE)
    {
        //Serial.println(previousMillisConnection);
        previousMillisConnection = currentMillis;
        // Get the exoskeleton connection to the watch status
        //Serial.println("HMI update called: connected to exoskeleton");
        char * text = lv_label_get_text(menu.label_connection);
        // update the connection label
        if (true) // TODO: replace with the actual connection status check
        {
            if (strcmp(text, "Connecting") == 0)
            {
                lv_label_set_text(menu.label_connection, "Connecting.");
            }
            else if (strcmp(text, "Connecting.") == 0)
            {
                lv_label_set_text(menu.label_connection, "Connecting..");
            }
            else if (strcmp(text, "Connecting..") == 0)
            {
                lv_label_set_text(menu.label_connection, "Connecting...");
            }
            else if (strcmp(text, "Connecting...") == 0)
            {
                lv_label_set_text(menu.label_connection, "Connecting");
            }
            else
            {
                lv_label_set_text(menu.label_connection, "Unknown error");
            }
        }
        else 
        {
            lv_label_set_text(menu.label_connection, "Connected");
        }
    }

    if (currentMillis - previousMillisBattery >= BATTERY_TIME_LAPSE)
    {
        previousMillisBattery = currentMillis;
        updateBatteryLabel();
    }

    lv_task_handler();
}

// button event handlers definitions
void addMassEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Call the add mass function
        HMI::getInstance()->addMass();
    }
}

void subMassEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Call the add mass function
        HMI::getInstance()->subMass();
    }
}

void addChargeMassEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Call the add mass function
        HMI::getInstance()->addChargeMass();
    }
}

void subChargeMassEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Call the add mass function
        HMI::getInstance()->subChargeMass();
    }
}

void addHeightEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Call the add mass function
        HMI::getInstance()->addHeight();
    }
}

void subHeightEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Call the add mass function
        HMI::getInstance()->subHeight();
    }
}

void toggle_SAE_SI_Event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Call the add mass function
        HMI::getInstance()->toggle_SAE_SI();
    }
}

void resetEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        // Call the add mass function
        HMI::getInstance()->resetExoSettings();
    }
}
