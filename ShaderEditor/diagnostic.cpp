////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostic.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "diagnostic.h"
#include "light.h"
#include <assert.h>

Diagnostic::Diagnostic(std::vector<Mesh::Instance>& diagnostics) :
    m_diagnostics(diagnostics)
{
}

Diagnostic::InstanceUpdater::InstanceUpdater(const Float3& Position, 
                                             const Colour& Colour) :
    position(Position),
    colour(Colour)
{
}

void Diagnostic::Tick()
{
    assert(m_diagnostics.size() == m_updater.size());

    for (unsigned int i = 0; i < m_diagnostics.size(); ++i)
    {
        if (m_diagnostics[i].shouldRender)
        {
            m_diagnostics[i].colour = m_updater[i].colour;
            m_diagnostics[i].position = m_updater[i].position;
        }
    }
}

int Diagnostic::AddInstance(const Float3& position, const Colour& colour, float scale)
{
    const int index = m_diagnostics.size();
    m_diagnostics.emplace_back();
    m_updater.emplace_back(position, colour);
    m_diagnostics[index].scale = scale;
    return index;
}

void Diagnostic::AddInstance(const Light& light, float scale)
{
    const int index = AddInstance(light.position, light.diffuse, scale);
    EnableInstance(index, m_lightDiagnosticsOn);
    m_lights.push_back(index);
}

void Diagnostic::EnableInstance(int index, bool enabled)
{
    m_diagnostics[index].shouldRender = enabled;
}

void Diagnostic::ToggleLightDiagnostics()
{
    m_lightDiagnosticsOn = !m_lightDiagnosticsOn;
    for(int index : m_lights)
    {
        EnableInstance(index, m_lightDiagnosticsOn);
    }
}