////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - lightmanager.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "lightmanager.h"
#include <AntTweakBar.h>

Light::Light() :
    castshadow(false)
{
    diffuse.assign(0.0f);
    specular.assign(0.0f);
}

LightManager::LightManager(CTwBar* tweakbar)
{
    const std::string lightfile = ASSETS_PATH + "Lights.xml";














}
