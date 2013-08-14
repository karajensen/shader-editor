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

const std::array<float, ShaderEditor::MAX_EDITABLE>& ShaderEditor::GetEditableComponents() const
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
        return "BUMP MAPPING";
    case SPECULAR_VISIBILITY:
        return "SPECULAR SHADING";
    case SOFTSHADOW_VISIBILITY:
        return "SOFT SHADOWS";
    case PARALLAX_VISIBILITY:
        return "PARALLAX MAPPING";
    default:
        return "NONE";
    };
}