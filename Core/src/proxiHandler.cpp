#include "proxiHandler.h"

ProxiHandler::ProxiHandler()
{
    this->mux = new Multiplex();
    this->LeftProxim = new ProxiSensor(mux, LEFT_MOUSTACHE_MUX_CHANNEL);
    this->RightProxim = new ProxiSensor(mux, RIGHT_MOUSTACHE_MUX_CHANNEL);
}

void ProxiHandler::read()
{
    //check if proxim to reset trigger dist
    if(dataCore.getResetProxim())
    {
        setTriggerDist();
        dataCore.setResetProxim(false);
    }
    dataCore.setRightProxi(RightProxim->IsOnTheGround());
    dataCore.setLeftProxi(LeftProxim->IsOnTheGround());
}

void ProxiHandler::setTriggerDist(){
    LeftProxim->UpdateSettings(dataCore.getBrightness());
    RightProxim->UpdateSettings(dataCore.getBrightness());
}

void ProxiHandler::print(){
    Serial.print("\t RIGHT PROXIM: ");
    Serial.print(dataCore.getRightProxi());
    Serial.print("\t LEFT PROXIM: ");
    Serial.println(dataCore.getLeftProxi());
}
