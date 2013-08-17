////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - lighteditor.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "lighteditor.h"
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

namespace
{
    const std::string FILE_EXT = ".xml"; ///< Extension for the lights file
    const std::string FILE_NAME = "Lights"; ///< File name for the lights file
}

LightEditor::Light::Light(ILightSceneNode* lightNode, const std::string& lightName) : 
    node(lightNode),
    name(lightName)
{
}

LightEditor::LightEditor(EnginePtr engine) :
    m_selectedIndex(NO_INDEX),
    m_engine(engine)
{
    using namespace boost;
    std::string lightsName = FILE_NAME+FILE_EXT;
    filesystem::path filePath(ASSETS_PATH+lightsName);
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
                SLight light;

                std::string lightype = it->second.get_child("Type").data();
                if(boost::algorithm::iequals(lightype, "Directional"))
                {
                    light.Type = ELT_DIRECTIONAL;
                }
                else if(boost::algorithm::iequals(lightype, "Point"))
                {
                    light.Type = ELT_POINT;
                }
                else
                {
                    Logger::LogError("Light type not set");
                }

                light.DiffuseColor.a = 1.0f;
                light.DiffuseColor.r = GetPtreeValue(it,1.0f,"R");
                light.DiffuseColor.b = GetPtreeValue(it,1.0f,"B");
                light.DiffuseColor.g = GetPtreeValue(it,1.0f,"G");
                light.SpecularColor.a = 1.0f;
                light.SpecularColor.r = GetPtreeValue(it,1.0f,"SR");
                light.SpecularColor.b = GetPtreeValue(it,1.0f,"SB");
                light.SpecularColor.g = GetPtreeValue(it,1.0f,"SG");
                light.Position.X = GetPtreeValue(it,0.0f,"X");
                light.Position.Y = GetPtreeValue(it,0.0f,"Y");
                light.Position.Z = GetPtreeValue(it,0.0f,"Z");
                light.Falloff = GetPtreeValue(it,5.0f,"Specularity");
                light.CastShadows = GetPtreeValue(it,false,"Shadows");
                light.Attenuation.X = GetPtreeValue(it,0.0f,"AttX");
                light.Attenuation.Y = GetPtreeValue(it,0.0f,"AttY");
                light.Attenuation.Z = GetPtreeValue(it,0.0f,"AttZ");
        
                ILightSceneNode* node = m_engine->scene->addLightSceneNode();
                node->setLightData(light);

                m_lights.push_back(Light(node, GetPtreeValue(it, 
                    std::string("UNNAMED"), "Name")));
            }
        }
        catch(const filesystem::filesystem_error& e)
        {
            Logger::LogError(e.what());
        }
    }
    else
    {
        Logger::LogError("Could not find " + ASSETS_PATH + lightsName);
    }    

    // Fill in the selected light data
    SelectNextLight();
}

const char* LightEditor::GetLightType() const
{
    return m_lights[m_selectedIndex].name.c_str();
}

LightEditor::LightData& LightEditor::GetSelectedLightData()
{
    return m_selectedData;
}

void LightEditor::Update()
{
    SLight& data = m_lights[m_selectedIndex].node->getLightData();
    data.Attenuation.X = m_selectedData.attenuation[0];
    data.Attenuation.Y = m_selectedData.attenuation[1];
    data.Attenuation.Z = m_selectedData.attenuation[2];
    data.Position.X = m_selectedData.position[0];
    data.Position.Y = m_selectedData.position[1];
    data.Position.Z = m_selectedData.position[2];
    data.DiffuseColor.r = m_selectedData.diffuse[0];
    data.DiffuseColor.g = m_selectedData.diffuse[1];
    data.DiffuseColor.b = m_selectedData.diffuse[2];
    data.SpecularColor.r = m_selectedData.specular[0];
    data.SpecularColor.g = m_selectedData.specular[1];
    data.SpecularColor.b = m_selectedData.specular[2];
    data.CastShadows = m_selectedData.castsShadows;
    m_lights[m_selectedIndex].node->setLightData(data);
    m_lights[m_selectedIndex].node->setPosition(data.Position);
}

void LightEditor::SelectNextLight()
{
    ++m_selectedIndex;
    if(m_selectedIndex >= static_cast<int>(m_lights.size()))
    {
        m_selectedIndex = 0;
    }

    const SLight& data = m_lights[m_selectedIndex].node->getLightData();
    m_selectedData.attenuation[0] = data.Attenuation.X;
    m_selectedData.attenuation[1] = data.Attenuation.Y;
    m_selectedData.attenuation[2] = data.Attenuation.Z;
    m_selectedData.position[0] = data.Position.X;
    m_selectedData.position[1] = data.Position.Y;
    m_selectedData.position[2] = data.Position.Z;
    m_selectedData.diffuse[0] = data.DiffuseColor.r;
    m_selectedData.diffuse[1] = data.DiffuseColor.g;
    m_selectedData.diffuse[2] = data.DiffuseColor.b;
    m_selectedData.specular[0] = data.SpecularColor.r;
    m_selectedData.specular[1] = data.SpecularColor.g;
    m_selectedData.specular[2] = data.SpecularColor.b;
    m_selectedData.castsShadows = data.CastShadows;
}

void LightEditor::SaveLightsToFile()
{
    using namespace boost;
    std::string fileName = FILE_NAME+FILE_EXT;
    filesystem::path filePath(ASSETS_PATH+fileName);
    if(filesystem::exists(filePath))
    {
        filesystem::path prevPath(ASSETS_PATH+FILE_NAME+"_Previous"+FILE_EXT);
        if(filesystem::exists(prevPath))
        {
            filesystem::remove(prevPath);
        }
        filesystem::rename(filePath,prevPath);
    }

    property_tree::ptree root,lights;
    std::vector<property_tree::ptree> light;
    for(unsigned int i = 0; i < m_lights.size(); ++i)
    {
        const SLight& data = m_lights[i].node->getLightData();
        light.push_back(property_tree::ptree());

        std::string name(m_lights[i].name.begin(), m_lights[i].name.end());
        light[i].add("Name",name.c_str());

        switch(data.Type)
        {
        case ELT_POINT:
            light[i].add("Type","Point");
            break;
        case ELT_DIRECTIONAL:
            light[i].add("Type","Directional");
            break;
        }
        light[i].add("X",data.Position.X);
        light[i].add("Y",data.Position.Y);
        light[i].add("Z",data.Position.Z);
        light[i].add("R",data.DiffuseColor.r);
        light[i].add("G",data.DiffuseColor.g);
        light[i].add("B",data.DiffuseColor.b);
        light[i].add("SR",data.SpecularColor.r);
        light[i].add("SB",data.SpecularColor.g);
        light[i].add("SG",data.SpecularColor.b);
        light[i].add("AttX",data.Attenuation.X);
        light[i].add("AttY",data.Attenuation.Y);
        light[i].add("AttZ",data.Attenuation.Z);
        light[i].add("Shadows",data.CastShadows?"1":"0");
        lights.add_child("Light", light[i]);
    }
    root.add_child("Lights",lights);

    // Writing property tree to xml
    property_tree::xml_parser::xml_writer_settings<char> settings('\t', 1);
    property_tree::write_xml(filePath.generic_string(), root, std::locale(), settings);
}
