////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - lightmanager.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "lightmanager.h"
#include "AntTweakBar.h"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/algorithm/string.hpp"

namespace
{
    const std::string GROUPNAME = " group='Lighting' "; ///< Group name for the light tweak bar entry

    /**
    * Anttweakbar button callback for selecting the next light
    * @param clientData Light manager object
    */
    //void TW_CALL SelectNextLightButton(void *clientData)
    //{
    //    static_cast<LightManager*>(clientData)->SelectNextLight();
    //}
}

Light::Light() :
    castshadow(false),
    specularity(0.0f)
{
}

LightManager::LightManager() :
    m_selectedLight(NO_INDEX)
{
}

bool LightManager::Initialise()
{
    using namespace boost;
    filesystem::path filePath(ASSETS_PATH + "Lights.xml");
    if(filesystem::exists(filePath))
    {
        try
        {
            property_tree::ptree root;
            property_tree::xml_parser::read_xml(filePath.generic_string(), 
                root, property_tree::xml_parser::trim_whitespace);
            property_tree::ptree& tree = root.get_child("Lights");

            boost::property_tree::ptree::iterator it;
            for(it = tree.begin(); it != tree.end(); ++it)
            {
                Light light;
                light.name = GetPtreeValue(it, std::string("UNNAMED"), "Name");

                light.diffuse.a = 1.0f;
                light.diffuse.r = GetPtreeValue(it,1.0f,"R");
                light.diffuse.b = GetPtreeValue(it,1.0f,"B");
                light.diffuse.g = GetPtreeValue(it,1.0f,"G");

                light.specular.a = 1.0f;
                light.specular.r = GetPtreeValue(it,1.0f,"SR");
                light.specular.b = GetPtreeValue(it,1.0f,"SB");
                light.specular.g = GetPtreeValue(it,1.0f,"SG");

                light.position.x = GetPtreeValue(it,0.0f,"X");
                light.position.y = GetPtreeValue(it,0.0f,"Y");
                light.position.z = GetPtreeValue(it,0.0f,"Z");

                light.specularity = GetPtreeValue(it,5.0f,"Specularity");
                light.castshadow = GetPtreeValue(it,false,"Shadows");
                light.attenuation.x = GetPtreeValue(it,0.0f,"AttX");
                light.attenuation.y = GetPtreeValue(it,0.0f,"AttY");
                light.attenuation.z = GetPtreeValue(it,0.0f,"AttZ");

                m_lights.push_back(light);
            }
        }
        catch(const filesystem::filesystem_error& e)
        {
            Logger::LogError(std::string("Lighting: ") + e.what());
            return false;
        }
    }
    else
    {
        Logger::LogError("Lighting: Could not find file");
        return false;
    }    

    Logger::LogInfo("Lighting: Initialised successfully");
    return true;
}

void LightManager::SelectNextLight()
{
    ++m_selectedLight;
    if(m_selectedLight >= static_cast<int>(m_lights.size()))
    {
        m_selectedLight = 0;
    }
}