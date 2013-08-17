////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shadereditor.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "shadereditor.h"
#include <boost/assign/list_of.hpp>

ShaderEditor::ShaderEditor()
{
    m_editableComponents.assign(1.0f);
}

std::vector<std::string> ShaderEditor::GetComponentDescriptions() const
{
    return boost::assign::list_of<std::string>
        (GetComponentDescription(FLAT))
        (GetComponentDescription(BUMP))
        (GetComponentDescription(SPECULAR))
        (GetComponentDescription(ALPHA))
        (GetComponentDescription(PARALLAX));
}

void* ShaderEditor::GetComponentAddress(ShaderEditor::EditableComponent component)
{
    return &m_editableComponents[component];
}

const ShaderEditor::EditableComponentArray& ShaderEditor::GetEditableComponents() const
{
    return m_editableComponents;
}

void ShaderEditor::SetComponentVisibility(unsigned int component, float value)
{
    m_editableComponents[component] = value;
}

std::string ShaderEditor::GetComponentDescription(Component component) const
{
    switch(component)
    {
    case FLAT:
        return "FLAT";
    case BUMP:
        return "BUMP";
    case SPECULAR:
        return "SPECULAR";
    case ALPHA:
        return "ALPHA";
    case PARALLAX:
        return "PARALLAX";
    case SHADOW:
        return "SHADOW";
    default:
        return "NONE";
    };
}

const char* ShaderEditor::GetComponentDescription(EditableComponent component) const
{
    switch(component)
    {
    case BUMP_VISIBILITY:
        return "Bump";
    case SPECULAR_VISIBILITY:
        return "Specular";
    case SOFTSHADOW_VISIBILITY:
        return "Shadows";
    case PARALLAX_VISIBILITY:
        return "Parallax";
    default:
        return "None";
    };
}