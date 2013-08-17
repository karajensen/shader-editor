////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shadereditor.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include <array>
#include <boost/noncopyable.hpp>

/**
* Generated shader component edtior
*/
class ShaderEditor : boost::noncopyable
{
public:

    /**
    * All shader components
    */
    enum Component
    {
        FLAT,
        BUMP,
        SPECULAR,
        ALPHA,
        PARALLAX,
        SHADOW
    };

    /**
    * Shader Components avaliable for editing
    */
    enum EditableComponent
    {
        SPECULAR_VISIBILITY,
        BUMP_VISIBILITY,
        PARALLAX_VISIBILITY,
        SOFTSHADOW_VISIBILITY,
        MAX_EDITABLE
    };

    /**
    * Constructor
    */
    ShaderEditor();

    /**
    * @return a vector of all shader component names
    */
    std::vector<std::string> GetComponentDescriptions() const;

    /**
    * Sets the visiblity of the editable component
    * @param component The component to set
    * @param value The level of visibilty of the component
    */
    void SetComponentVisibility(unsigned int component, float value);

    /**
    * @param component The component to get a description for
    * @return a string description of the editable component
    */
    const char* GetComponentDescription(EditableComponent component) const;

    /**
    * @param component The component to get a description for
    * @return a string description of the component
    */
    std::string GetComponentDescription(Component component) const;

    /**
    * @return an  array of editable component visibilities
    */
    typedef std::array<float, MAX_EDITABLE> EditableComponentArray;
    const EditableComponentArray& GetEditableComponents() const;

    /**
    * Allows diagnostics to use the editable components in a tweak bar
    * @param component The component to access
    * @return a the address of the editable component
    */
    void* GetComponentAddress(EditableComponent component);


private:

    EditableComponentArray m_editableComponents;  ///< components that can be edited
};