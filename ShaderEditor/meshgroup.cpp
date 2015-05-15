////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshgroup.h
////////////////////////////////////////////////////////////////////////////////////////

#include "meshgroup.h"

void MeshGroup::AddMesh(int index, int instance)
{
    const auto mesh = m_group.size();
    m_group.emplace_back();
    m_group[mesh].index = index;
    m_group[mesh].instance = instance;
}

const std::vector<InstanceKey>& MeshGroup::GetKeys() const
{
    return m_group;
}

void MeshGroup::AddShadow(int shadow)
{
    m_shadowID = shadow;
}

int MeshGroup::GetShadow() const
{
    return m_shadowID;
}