#include "light_editor.h"
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

LightEditor::LightPtr LightEditor::sm_lightEditor;

LightEditor::LightEditor() :
    m_selectedLight(0)
{
}

bool LightEditor::Initialise()
{
    sm_lightEditor.reset(new LightEditor());

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
                    return false;
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
        
                ILightSceneNode* node = Scene()->addLightSceneNode();
                node->setLightData(light);

                sm_lightEditor->m_lights.push_back(Light(node, GetPtreeValue(it, 
                    std::string("UNNAMED"), "Name")));
            }
        }
        catch(const filesystem::filesystem_error& e)
        {
            Logger::LogError(e.what());
            return false;
        }
    }
    else
    {
        Logger::LogError("Could not find " + ASSETS_PATH + lightsName);
        return false;
    }    
    return true;
}

const SLight& LightEditor::GetSelectedLightData() const
{
    return m_lights[m_selectedLight].node->getLightData();
}

stringw LightEditor::GetSelectedLightDescription() const
{
    stringw description(m_lights[m_selectedLight].name.c_str());
    description.make_upper();

    const auto& data = GetSelectedLightData();
    switch(data.Type)
    {
    case ELT_DIRECTIONAL:
        description += " \n[DIRECTIONAL]";
        break;
    case ELT_POINT:
        description += " \n[POINT]";
        break;
    }
    return description;
}

stringw LightEditor::GetAttributeDescription(unsigned int attribute) const
{
    switch(attribute)
    {
        case ATTENUATION_X: 
            return "ATTENUATION X";
        case ATTENUATION_Y:
            return "ATTENUATION Y";
        case ATTENUATION_Z:
            return "ATTENUATION Z";
        case POSITION_X:
            return "POSITION X";
        case POSITION_Y:    
            return "POSITION Y";
        case POSITION_Z:    
            return "POSITION Z";
        case COLOR_R:  
            return "DIFFUSE R";
        case COLOR_G:       
            return "DIFFUSE G";
        case COLOR_B:       
            return "DIFFUSE B";
        case SPECCOLOR_R:  
            return "SPECULAR R";
        case SPECCOLOR_G:       
            return "SPECULAR G";
        case SPECCOLOR_B:       
            return "SPECULAR B";
        case SHADOWS:
            return "SHADOWS";
        default:
            return "None";
    }
}

void LightEditor::SetAttributeValue(unsigned int attribute, float value) 
{
    SLight& data = m_lights[m_selectedLight].node->getLightData();
    switch(attribute)
    {
        case ATTENUATION_X: 
            data.Attenuation.X = value;
            break;
        case ATTENUATION_Y:
            data.Attenuation.Y = value;
            break;
        case ATTENUATION_Z:
            data.Attenuation.Z = value;
            break;
        case POSITION_X:
            data.Position.X = value;
            m_lights[m_selectedLight].node->setPosition(data.Position);
            break;
        case POSITION_Y:    
            data.Position.Y = value;
            m_lights[m_selectedLight].node->setPosition(data.Position);
            break;
        case POSITION_Z:    
            data.Position.Z = value;
            m_lights[m_selectedLight].node->setPosition(data.Position);
            break;
        case COLOR_R:  
            data.DiffuseColor.r = value;
            break;
        case COLOR_G:       
            data.DiffuseColor.g = value;
            break;
        case COLOR_B:       
            data.DiffuseColor.b = value;
            break;
        case SPECCOLOR_R:  
            data.SpecularColor.r = value;
            break;
        case SPECCOLOR_G:       
            data.SpecularColor.g = value;
            break;
        case SPECCOLOR_B:       
            data.SpecularColor.b = value;
            break;
        case SHADOWS:
            data.CastShadows = (value != 0);
    }
}

void LightEditor::SelectNextLight()
{
    ++m_selectedLight;
    if(m_selectedLight >= m_lights.size())
    {
        m_selectedLight = 0;
    }
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
