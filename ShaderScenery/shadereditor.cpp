////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shadereditor.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "shadereditor.h"
#include <boost/assign/list_of.hpp>

ShaderEditor::ShaderEditor()
{
    m_editableComponents.assign(1.0f);
}

std::vector<std::string> ShaderEditor::GetComponentDescriptions()
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

std::string ShaderEditor::GetComponentDescription(Component component)
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

std::string ShaderEditor::GetComponentDescription(EditableComponent component)
{
    switch(component)
    {
    case BUMP_VISIBILITY:
        return "Bump Mapping";
    case SPECULAR_VISIBILITY:
        return "Specular Shading";
    case SOFTSHADOW_VISIBILITY:
        return "Soft Shadows";
    case PARALLAX_VISIBILITY:
        return "Parallax Mapping";
    default:
        return "NONE";
    };
}