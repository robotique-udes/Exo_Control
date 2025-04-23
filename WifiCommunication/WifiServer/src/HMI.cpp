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
    setupMenu();
    setupWatchSettings();
    setupExoSettings();
    setupInfo();
    setupProfiles();
}

void HMI::setupMenu()
{
    Serial.println("HMI setupMenu called");
}

void HMI::setupWatchSettings()
{
    Serial.println("HMI setupWatchSettings called");
}

void HMI::setupExoSettings()
{
    Serial.println("HMI setupExoSettings called");
}

void HMI::setupInfo()
{
    Serial.println("HMI setupInfo called");
}

void HMI::setupProfiles()
{
    Serial.println("HMI setupProfiles called");
}
