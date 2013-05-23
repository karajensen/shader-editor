#include "light_editor.h"
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

namespace
{
    const float ATT_INC = 0.0001f;  ///< Increment for attenuation
    const float POS_INC = 0.1f;     ///< Increment for position
    const float COLOR_INC = 0.001f; ///< Increment for colors
    const std::string FILE_EXT = ".xml"; ///< Extension for the lights file
    const std::string FILE_NAME = "Lights"; ///< File name for the lights file
}

LightEditor::LightEditor() :
    m_selectedLight(NO_INDEX),
    m_selectedAtt(ATTENUATION_X)
{
}

bool LightEditor::Initialise(const std::string& assetspath)
{
    using namespace boost;
    std::string lightsName = FILE_NAME+FILE_EXT;
    filesystem::path filePath(assetspath+lightsName);
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

                m_lights.push_back(Light(node,
                    GetPtreeValue(it,std::string("UNNAMED"),"Name")));
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
        Logger::LogError("Could not find " + assetspath + lightsName);
        return false;
    }    
    return true;
}

void LightEditor::UpdateLightDiagnosticText()
{
    if(m_selectedLight != NO_INDEX)
    {
        const SLight& data = m_lights[m_selectedLight].node->getLightData();

        m_editText = "LIGHT: ";
        m_editText += m_lights[m_selectedLight].name.c_str();
        m_editText += "\n SHADOWS: ";
        m_editText += data.CastShadows;
        m_editText += "\n TYPE: ";
    
        switch(data.Type)
        {
        case ELT_POINT:
            m_editText += "Point";
            break;
        case ELT_DIRECTIONAL:
            m_editText += "Directional";
            break;
        }
        m_editText += "\n ATT0: ";
        m_editText += data.Attenuation.X;
        m_editText += "\n ATT1: ";
        m_editText += data.Attenuation.Y;
        m_editText += "\n ATT2: ";
        m_editText += data.Attenuation.Z;
        m_editText += "\n X: ";
        m_editText += data.Position.X;
        m_editText += "\n Y: ";
        m_editText += data.Position.Y;
        m_editText += "\n Z: ";
        m_editText += data.Position.Z;
        m_editText += "\n R: ";
        m_editText += data.DiffuseColor.r;
        m_editText += "\n G: ";
        m_editText += data.DiffuseColor.g;
        m_editText += "\n B: ";
        m_editText += data.DiffuseColor.b;
        m_editText += "\n SR: ";
        m_editText += data.SpecularColor.r;
        m_editText += "\n SG: ";
        m_editText += data.SpecularColor.g;
        m_editText += "\n SB: ";
        m_editText += data.SpecularColor.b;

        m_editText += "\n \n SELECTED ATT:\n ";
        switch(m_selectedAtt)
        {
            case ATTENUATION_X: 
                m_editText += " ATTENUATION_X\n  "; 
                m_editText += data.Attenuation.X;
                break;
            case ATTENUATION_Y:
                m_editText += " ATTENUATION_Y\n  "; 
                m_editText += data.Attenuation.Y;
                break;
            case ATTENUATION_Z: 
                m_editText += " ATTENUATION_Z\n  "; 
                m_editText += data.Attenuation.Z;
                break;
            case POSITION_X:   
                m_editText += " POSITION_X\n  "; 
                m_editText += data.Position.X;
                break;
            case POSITION_Y:    
                m_editText += " POSITION_Y\n  "; 
                m_editText += data.Position.Y;
                break;
            case POSITION_Z:    
                m_editText += " POSITION_Z\n  "; 
                m_editText += data.Position.Z;
                break;
            case COLOR_R:       
                m_editText += " COLOR_R\n  ";  
                m_editText += data.DiffuseColor.r;
                break;
            case COLOR_G:       
                m_editText += " COLOR_G\n  ";     
                m_editText += data.DiffuseColor.g;
                break;
            case COLOR_B:       
                m_editText += " COLOR_B\n  ";   
                m_editText += data.DiffuseColor.b;
                break;
            case SPECCOLOR_R:   
                m_editText += " SPECCOLOR_R\n  ";   
                m_editText += data.SpecularColor.r;
                break;
            case SPECCOLOR_G:  
                m_editText += " SPECCOLOR_G\n  ";
                m_editText += data.SpecularColor.g;
                break;
            case SPECCOLOR_B:   
                m_editText += " SPECCOLOR_B\n  ";  
                m_editText += data.SpecularColor.b;
                break;
        }
    }
}

const irr::core::stringw& LightEditor::GetEditorText() const
{
    return m_editText;
}

void LightEditor::IncrementAtt(bool incrementUp)
{
    if(m_selectedLight != NO_INDEX)
    {
        SLight& data = m_lights[m_selectedLight].node->getLightData();
        switch(m_selectedAtt)
        {
            case ATTENUATION_X: 
                data.Attenuation.X += (incrementUp ? ATT_INC : -ATT_INC);
                break;
            case ATTENUATION_Y:
                data.Attenuation.Y += (incrementUp ? ATT_INC : -ATT_INC);
                break;
            case ATTENUATION_Z:
                data.Attenuation.Z += (incrementUp ? ATT_INC : -ATT_INC);
                break;
            case POSITION_X:
                data.Position.X += (incrementUp ? POS_INC : -POS_INC);
                MoveSelectedLight(vector3df((incrementUp ? POS_INC : -POS_INC),0.0f,0.0f));
                break;
            case POSITION_Y:    
                data.Position.Y += (incrementUp ? POS_INC : -POS_INC);
                MoveSelectedLight(vector3df(0.0f,(incrementUp ? POS_INC : -POS_INC),0.0f));
                break;
            case POSITION_Z:    
                data.Position.Z += (incrementUp ? POS_INC : -POS_INC);
                MoveSelectedLight(vector3df(0.0f,0.0f,(incrementUp ? POS_INC : -POS_INC)));
                break;
            case COLOR_R:  
                data.DiffuseColor.r += (incrementUp ? COLOR_INC : -COLOR_INC);
                break;
            case COLOR_G:       
                data.DiffuseColor.g += (incrementUp ? COLOR_INC : -COLOR_INC);
                break;
            case COLOR_B:       
                data.DiffuseColor.b += (incrementUp ? COLOR_INC : -COLOR_INC);
                break;
            case SPECCOLOR_R:  
                data.SpecularColor.r += (incrementUp ? COLOR_INC : -COLOR_INC);
                break;
            case SPECCOLOR_G:       
                data.SpecularColor.g += (incrementUp ? COLOR_INC : -COLOR_INC);
                break;
            case SPECCOLOR_B:       
                data.SpecularColor.b += (incrementUp ? COLOR_INC : -COLOR_INC);
                break;
        }
        UpdateLightDiagnosticText();
    }
}

void LightEditor::ToggleShadows()
{
    if(m_selectedLight != NO_INDEX)
    {
        m_lights[m_selectedLight].node->getLightData().CastShadows = 
            !m_lights[m_selectedLight].node->getLightData().CastShadows;
        UpdateLightDiagnosticText();
    }
}

void LightEditor::SelectNextLightAtt(bool travelDown)
{
    if(m_selectedLight != NO_INDEX)
    {
        if(travelDown)
        {
            int nextAtt = m_selectedAtt+1;
            nextAtt = (nextAtt >= MAX_ATTRIBUTES) ? 0 : nextAtt;
            m_selectedAtt = static_cast<Attribute>(nextAtt);
        }
        else
        {
            int nextAtt = m_selectedAtt-1;
            nextAtt = (nextAtt < 0) ? MAX_ATTRIBUTES-1 : nextAtt;
            m_selectedAtt = static_cast<Attribute>(nextAtt);

        }
        UpdateLightDiagnosticText();
    }
}

void LightEditor::SelectNextLight()
{
    ++m_selectedLight;
    if(m_selectedLight >= static_cast<int>(m_lights.size()))
    {
        m_selectedLight = NO_INDEX;
        m_editText = "";
    }
    else
    {
        UpdateLightDiagnosticText();
    }
}

void LightEditor::MoveSelectedLight(const vector3df& direction)
{
    m_lights[m_selectedLight].node->setPosition(
        m_lights[m_selectedLight].node->getPosition()+direction);
    UpdateLightDiagnosticText();
}

void LightEditor::SaveLightsToFile(const std::string& assetspath)
{
    using namespace boost;
    std::string fileName = FILE_NAME+FILE_EXT;
    filesystem::path filePath(assetspath+fileName);
    if(filesystem::exists(filePath))
    {
        filesystem::path prevPath(assetspath+FILE_NAME+"_Previous"+FILE_EXT);
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

        light[i].add("Name",m_lights[i].name);
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

void LightEditor::ClearLights()
{
    m_editText = "";
    m_selectedLight = NO_INDEX;
    m_lights.clear();
}