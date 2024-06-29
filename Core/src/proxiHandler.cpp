#include "proxiHandler.h"

ProxiHandler::ProxiHandler()
{
    this->mux = Multiplex();
    this->LeftProxim = new ProxiSensor(&this->mux, LEFT_MOUSTACHE_MUX_CHANNEL);
    this->RightProxim = new ProxiSensor(&this->mux, RIGHT_MOUSTACHE_MUX_CHANNEL);
}

void ProxiHandler::read()
{
    dataCore.setRightProxi(RightProxim->IsOnTheGround()); 
    dataCore.setLeftProxi(LeftProxim->IsOnTheGround()); 

}

void ProxiHandler::setTriggerDist(){
    LeftProxim->SetTriggerDistance(dataCore.getBrightness());
    RightProxim->SetTriggerDistance(dataCore.getBrightness());
}
