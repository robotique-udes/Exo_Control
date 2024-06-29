#include "proxiHandler.h"

ProxiHandler::ProxiHandler()
{
    this->mux = Multiplex();
    this->LeftProxim = new ProxiSensor(&this->mux, LEFT_MOUSTACHE_MUX_CHANNEL);
    this->RightProxim = new ProxiSensor(&this->mux, RIGHT_MOUSTACHE_MUX_CHANNEL);
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
    LeftProxim->SetTriggerDistance(dataCore.getBrightness());
    RightProxim->SetTriggerDistance(dataCore.getBrightness());
}

void ProxiHandler::print(){
    Serial.print("\t RIGHT PROXIM: ");
    Serial.print(dataCore.getRightProxi());
    Serial.print("\t LEFT PROXIM: ");
    Serial.println(dataCore.getLeftProxi());
}
