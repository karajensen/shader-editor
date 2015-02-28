////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostic.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "mesh.h"
#include <unordered_map>

class Light;

/**
* Manages diagnostics for the scene
*/
class Diagnostic
{
public:

    /**
    * Constructor
    * @param diagnosticMesh The mesh to use for rendering diagnostics
    */
    Diagnostic(std::vector<Mesh::Instance>& diagnostics);

    /**
    * Updates the diagnostic instances
    */
    void Tick();

    /**
    * Adds an instance for diagnostics
    * @param position The position to update to
    * @param colour The colour to update to
    * @param scale The scale of the diagnostic
    * @return an index for the diagnostic
    */
    int AddInstance(const Float3& position, const Colour& colour, float scale);

    /**
    * Adds an instance for diagnostics
    * @param light The light to update to
    */
    void AddInstance(const Light& light, float scale);

    /**
    * Sets whether an instance should update and be visible
    */
    void EnableInstance(int index, bool enabled);

    /**
    * Sets whether all instances for light diagnostics should update and be visible
    */
    void ToggleLightDiagnostics();

private:

    /**
    * Holds information for updating an instance
    */
    struct InstanceUpdater
    {
        /**
        * Constructor
        */
        InstanceUpdater(const Float3& Position, const Colour& Colour);

        const Colour& colour;
        const Float3& position;
    };

    bool m_lightDiagnosticsOn = false; ///< Whether light diagnostics are visible
    std::vector<int> m_lights;  ///< Diagnostic indices for the lights
    std::vector<InstanceUpdater> m_updater; ///< Holds information for updating instances
    std::vector<Mesh::Instance>& m_diagnostics; ///< Container of instances for the mesh
};