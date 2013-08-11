////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include <boost/noncopyable.hpp>

/**
* Light manager and editor
*/
class LightEditor : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit LightEditor(EnginePtr engine);

    /**
    * Selects the next light in the light container or 
    * diables the editor if at end of container
    */
    void SelectNextLight();

    /**
    * Sets the given attribute to a value
    * @param attribute The attribute to set
    * @param value The value for the attribute
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

private:

    /**
    * Structure for diagnostic light information
    */
    struct Light
    {
        /**
        * Constructor
        * @param lightNode The node for the scene light
        * @param lightName The name for the scene light
        */
        Light(ILightSceneNode* lightNode, const std::string& lightName);

        ILightSceneNode* node;  ///< The node for the scene light
        std::wstring name;      ///< The name for the scene light
    };

    std::vector<Light> m_lights;    ///< Array of irrlicht poiners to lights
    unsigned int m_selectedLight;   ///< Currently selected light for diagnostics
    EnginePtr m_engine;             ///< Irrlicht engine
};