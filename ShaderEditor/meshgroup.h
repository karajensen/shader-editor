////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshgroup.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

/**
* Key for obtaining the instance of a mesh
*/
struct InstanceKey
{
    int index = -1;
    int instance = -1;
};

/**
* Group of meshes to share the same instance details
* @note assumes any meshes that share a group share the same pivot point
*/
class MeshGroup
{
public:

    /**
    * Adds a new mesh to the group and expands the radius
    * @param data The mesh data to add
    * @param index The ID of the mesh
    * @param instance The instance of the data to add
    */
    void AddMesh(int index, int instance);

    /**
    * @return the keys of the mesh group
    */
    const std::vector<InstanceKey>& GetKeys() const;

private:

    std::vector<InstanceKey> m_group;   ///< Keys for acessing the group
};
