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
    m_diagSelectedLight(NO_INDEX),
    m_selectedAtt(ATTENUATION_X),
    m_showDiagnostics(false)
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
    if(m_diagSelectedLight != NO_INDEX)
    {
        const SLight& data = m_lights[m_diagSelectedLight].node->getLightData();

        m_diagnosticText = "LIGHT: ";
        m_diagnosticText += m_lights[m_diagSelectedLight].name.c_str();
        m_diagnosticText += "\nSHADOWS: ";
        m_diagnosticText += data.CastShadows;
        m_diagnosticText += "\nTYPE: ";
    
        switch(data.Type)
        {
        case ELT_POINT:
            m_diagnosticText += "Point";
            break;
        case ELT_DIRECTIONAL:
            m_diagnosticText += "Directional";
            break;
        }
        m_diagnosticText += "\nATT0: ";
        m_diagnosticText += data.Attenuation.X;
        m_diagnosticText += "\nATT1: ";
        m_diagnosticText += data.Attenuation.Y;
        m_diagnosticText += "\nATT2: ";
        m_diagnosticText += data.Attenuation.Z;
        m_diagnosticText += "\nX: ";
        m_diagnosticText += data.Position.X;
        m_diagnosticText += "\nY: ";
        m_diagnosticText += data.Position.Y;
        m_diagnosticText += "\nZ: ";
        m_diagnosticText += data.Position.Z;
        m_diagnosticText += "\nR: ";
        m_diagnosticText += data.DiffuseColor.r;
        m_diagnosticText += "\nG: ";
        m_diagnosticText += data.DiffuseColor.g;
        m_diagnosticText += "\nB: ";
        m_diagnosticText += data.DiffuseColor.b;
        m_diagnosticText += "\nSR: ";
        m_diagnosticText += data.SpecularColor.r;
        m_diagnosticText += "\nSG: ";
        m_diagnosticText += data.SpecularColor.g;
        m_diagnosticText += "\nSB: ";
        m_diagnosticText += data.SpecularColor.b;

        m_diagnosticText += "\n\nSELECTED ATT:\n";
        switch(m_selectedAtt)
        {
            case ATTENUATION_X: 
                m_diagnosticText += " ATTENUATION_X\n "; 
                m_diagnosticText += data.Attenuation.X;
                break;
            case ATTENUATION_Y:
                m_diagnosticText += " ATTENUATION_Y\n "; 
                m_diagnosticText += data.Attenuation.Y;
                break;
            case ATTENUATION_Z: 
                m_diagnosticText += " ATTENUATION_Z\n "; 
                m_diagnosticText += data.Attenuation.Z;
                break;
            case POSITION_X:   
                m_diagnosticText += " POSITION_X\n "; 
                m_diagnosticText += data.Position.X;
                break;
            case POSITION_Y:    
                m_diagnosticText += " POSITION_Y\n "; 
                m_diagnosticText += data.Position.Y;
                break;
            case POSITION_Z:    
                m_diagnosticText += " POSITION_Z\n "; 
                m_diagnosticText += data.Position.Z;
                break;
            case COLOR_R:       
                m_diagnosticText += " COLOR_R\n ";  
                m_diagnosticText += data.DiffuseColor.r;
                break;
            case COLOR_G:       
                m_diagnosticText += " COLOR_G\n ";     
                m_diagnosticText += data.DiffuseColor.g;
                break;
            case COLOR_B:       
                m_diagnosticText += " COLOR_B\n ";   
                m_diagnosticText += data.DiffuseColor.b;
                break;
            case SPECCOLOR_R:   
                m_diagnosticText += " SPECCOLOR_R\n ";   
                m_diagnosticText += data.SpecularColor.r;
                break;
            case SPECCOLOR_G:  
                m_diagnosticText += " SPECCOLOR_G\n ";
                m_diagnosticText += data.SpecularColor.g;
                break;
            case SPECCOLOR_B:   
                m_diagnosticText += " SPECCOLOR_B\n ";  
                m_diagnosticText += data.SpecularColor.b;
                break;
        }
    }
}

const irr::core::stringw& LightEditor::GetDiagnosticText() const
{
    return m_diagnosticText;
}

bool LightEditor::RenderDiagnostics() const
{
    return m_showDiagnostics;
}

void LightEditor::IncrementAtt(bool incrementUp)
{
    if(m_diagSelectedLight != NO_INDEX)
    {
        SLight& data = m_lights[m_diagSelectedLight].node->getLightData();
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
    if(m_diagSelectedLight != NO_INDEX)
    {
        m_lights[m_diagSelectedLight].node->getLightData().CastShadows = 
            !m_lights[m_diagSelectedLight].node->getLightData().CastShadows;
        UpdateLightDiagnosticText();
    }
}

void LightEditor::SelectNextLightAtt(bool travelDown)
{
    if(m_diagSelectedLight != NO_INDEX)
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
    ++m_diagSelectedLight;
    if(m_diagSelectedLight >= static_cast<int>(m_lights.size()))
    {
        m_diagSelectedLight = NO_INDEX;
        m_showDiagnostics = false;
    }
    else
    {
        m_showDiagnostics = true;
        UpdateLightDiagnosticText();
    }
}

void LightEditor::MoveSelectedLight(const vector3df& direction)
{
    m_lights[m_diagSelectedLight].node->setPosition(
        m_lights[m_diagSelectedLight].node->getPosition()+direction);
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
    m_showDiagnostics = false;
    m_diagSelectedLight = NO_INDEX;
    m_lights.clear();
}