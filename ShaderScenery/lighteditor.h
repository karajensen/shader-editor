////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - lighteditor.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include <array>
#include <boost/noncopyable.hpp>

/**
* Light manager and editor
*/
class LightEditor : boost::noncopyable
{
public:

    /**
    * Data stored for the selected light
    * @note: various types are chosen to align with tweak bar
    */
    struct LightData
    {
        std::array<float,3> position;    ///< Position in world coord
        std::array<float,3> attenuation; ///< Falloff of light
        std::array<float,3> diffuse;     ///< Diffuse color
        std::array<float,3> specular;    ///< Specular color
        bool castsShadows;               ///< Whether casts shadows                      
    };

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit LightEditor(EnginePtr engine);

    /**
    * Saves the current lighting configuration to a file 
    * and renames the previous one to another name
    */
    void SaveLightsToFile();

    /**
    * Updates the selected light
    */
    void Update();

    /**
    * Selects the next light in the light container
    */
    void SelectNextLight();

    /**
    * @return the data for the selected light
    */
    LightData& GetSelectedLightData();

    /**
    * @return a description of the currently selected light
    */
    const char* GetLightType() const;

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
        std::string name;       ///< The name for the scene light
    };

    std::vector<Light> m_lights;  ///< Array of irrlicht poiners to lights
    EnginePtr m_engine;           ///< Irrlicht engine
    int m_selectedIndex;          ///< Currently selected light index for diagnostics
    LightData m_selectedData;     ///< Currently selected light data for diagnostics
};