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

Diagnostic::InstanceUpdater::InstanceUpdater(const Float3& Position) :
    position(Position)
{
}

void Diagnostic::Tick()
{
    assert(m_diagnostics.size() == m_updater.size());

    for (unsigned int i = 0; i < m_diagnostics.size(); ++i)
    {
        if (m_diagnostics[i].enabled)
        {
            m_diagnostics[i].position = m_updater[i].position;
            m_diagnostics[i].requiresUpdate = true;
        }
    }
}

int Diagnostic::AddInstance(const Float3& position, float scale)
{
    const int index = m_diagnostics.size();
    m_diagnostics.emplace_back();
    m_updater.emplace_back(position);
    m_diagnostics[index].scale.x = scale;
    m_diagnostics[index].scale.y = scale;
    m_diagnostics[index].scale.z = scale;
    return index;
}

void Diagnostic::AddInstance(const Light& light, float scale)
{
    const int index = AddInstance(light.Position(), scale);
    EnableInstance(index, m_lightDiagnosticsOn);
    m_lights.push_back(index);
}

void Diagnostic::EnableInstance(int index, bool enabled)
{
    m_diagnostics[index].enabled = enabled;
}

void Diagnostic::ToggleLightDiagnostics()
{
    m_lightDiagnosticsOn = !m_lightDiagnosticsOn;
    for(int index : m_lights)
    {
        EnableInstance(index, m_lightDiagnosticsOn);
    }
}