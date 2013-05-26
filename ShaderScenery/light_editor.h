/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* Static Editor for in-scene lights
*****************************************************************/
#pragma once

#include "common.h"
#include <boost/noncopyable.hpp>

class LightEditor : boost::noncopyable
{
public:

    /**
    * @return the pointer to the diagnostics object
    */
    typedef boost::shared_ptr<LightEditor> LightPtr;
    static const LightPtr& Get() { return sm_lightEditor; }

    /**
    * Loads in light data from a boost property tree
    * @return whether or not all light creation succeeded
    */
    static bool Initialise();

    /**
    * Selects the next light in the light container or 
    * diables the editor if at end of container
    */
    void SelectNextLight();

    /**
    * Adds or subtracts a value from the selected attribute
    * @param whether to add or not
    */
    void SetAttributeValue(unsigned int attribute, float value);

    /**
    * Saves the current lighting configuration to a file 
    * and renames the previous one to another name
    */
    void SaveLightsToFile();

    /**
    * @return the name for the selected light
    */
    stringw GetSelectedLightDescription() const;

    /**
    * @return the data for the selected light
    */
    const SLight& GetSelectedLightData() const;

    /**
    * @return description for the attribute
    */
    stringw GetAttributeDescription(unsigned int attribute) const;

    /**
    * Shared light attributes
    */
    enum Attribute
    {
        ATTENUATION_X,
        ATTENUATION_Y,
        ATTENUATION_Z,
        POSITION_X,
        POSITION_Y,
        POSITION_Z,
        COLOR_R,
        COLOR_G,
        COLOR_B,
        SPECCOLOR_R,
        SPECCOLOR_G,
        SPECCOLOR_B,
        SHADOWS,
        MAX_ATTRIBUTES
    };

    /**
    * Structure for diagnostic light information
    */
    struct Light
    {
        ILightSceneNode* node;
        std::wstring name;
        Light(ILightSceneNode* Node, const std::string& Name) : 
            node(Node),
            name(Name.begin(), Name.end())
        {
        }
    };

private:

    LightEditor();

    static LightPtr sm_lightEditor;   ///< Diagnostic singleton pointer
    std::vector<Light> m_lights;      ///< Array of irrlicht poiners to lights
    unsigned int m_selectedLight;     ///< Currently selected light for diagnostics
};