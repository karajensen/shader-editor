/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* Editor/Diagnostics for in-scene lights
*****************************************************************/
#pragma once

#include "common.h"

class LightEditor
{
public:

    LightEditor();

    /**
    * Loads in light data from a boost property tree
    * @param the path to where the lights xml is stored
    * @return whether or not all light creation succeeded
    */
    bool Initialise(const std::string& assetspath);

    /**
    * Selects the next light in the light container or 
    * diables the editor if at end of container
    */
    void SelectNextLight();

    /**
    * Selects the next light attribute for editing if a light is selected
    * @param whether to select up or down the list
    */
    void SelectNextLightAtt(bool travelDown);

    /**
    * Adds or subtracts a value from the selected attribute
    * @param whether to add or not
    */
    void IncrementAtt(bool incrementUp);

    /**
    * Moves the selected light in a specific direction 
    * @param the direction to move the light in
    */
    void MoveSelectedLight(const vector3df& direction);

    /**
    * @return the diagnostic text for the lights
    */
    const irr::core::stringw& GetDiagnosticText() const;

    /**
    * @return whether or not to render the light diagnostics
    */
    bool LightEditor::RenderDiagnostics() const;

    /**
    * Saves the current lighting configuration to a file 
    * and renames the previous one to another name
    * @param the path to where the lights xml is stored
    */
    void SaveLightsToFile(const std::string& assetspath);

    /**
    * Clears all lights
    */
    void ClearLights();

    /**
    * Toggles shadows for the currently selected light
    */
    void ToggleShadows();

private:

    /**
    * @return diagnostic text including name/positional 
    * information for the selected light
    */
    void UpdateLightDiagnosticText();  

    /**
    * Structure for diagnostic light information
    */
    struct Light
    {
        ILightSceneNode* node;
        std::string name;
        Light(ILightSceneNode* Node, const std::string& Name) : 
            node(Node), name(Name) 
        {
        }
    };

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
        MAX_ATTRIBUTES
    };

    std::vector<Light> m_lights;         ///< Array of irrlicht poiners to lights
    int m_diagSelectedLight;             ///< Currently selected light for diagnostics
    Attribute m_selectedAtt;             ///< Currently select attribute
    bool m_showDiagnostics;              ///< Whether to render light diagnostics
    irr::core::stringw m_diagnosticText; ///< Diagnostic text for lights
};