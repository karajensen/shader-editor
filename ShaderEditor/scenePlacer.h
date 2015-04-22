////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scenePlacer.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "float3"
#include "int2.h"
#include <vector>

class Terrain;
class Water;
struct SceneData;

/**
* Updates the scene depending on the camera position
* Holds information about the area which consists of patches of tiled water/sand
* Assumes water and sand patches are aligned and the same size
*/
class ScenePlacer
{
public:

    /**
    * Constructor
    * @param data Data for manipulating the scene
    */
    ScenePlacer(SceneData& data);

    /**
    * Destructor
    */
    ~ScenePlacer();

    /**
    * Initialises the scene
    * @param camera The position of the camera
    * @return whether initialisation was successful
    */
    bool Initialise(const Float3& camera);

    /**
    * Updates the scene
    * @param camera The position of the camera
    */
    void Update(const Float3& cameraPosition);

    /**
    * Resets the patches including foliage and emitter placement
    */
    void ResetPatches();

private:

    /**
    * Prevent copying
    */
    ScenePlacer(const ScenePlacer&) = delete;
    ScenePlacer& operator=(const ScenePlacer&) = delete;

    /**
    * Updates the patch to a new position
    * @param row/column The row and column of the patch
    * @param direction The direction the patch is moving
    * @note assumes direction is either [0,1] or [1,0]
    */
    void UpdatePatch(int row,
                     int column,
                     const Int2& direction);

    /**
    * Determines which patch the positin is inside
    * @param position the position the use
    * @note y component of position is unused
    * @return the patch the position is inside
    */
    Int2 GetPatchInside(const Float3& position) const;

    /**
    * @return the index from the row/column
    */
    int Index(int row, int column) const;

    /**
    * @return whether the given position is inside the patch
    * @param position the position the use
    * @param row/column The position of the patch to test
    * @note y component of position is unused
    * @return if the position is inside the pathc
    */
    bool IsInsidePatch(const Float3& position, int row, int column) const;

    /**
    * Shifts the patches so the camera is always in the center of the scene
    * @param direction The direction the camera is moving in the patch grid
    * @note assumes direction is either [0,1] or [1,0]
    */
    void ShiftPatches(const Int2& direction);

    /**
    * Assigns foliage and rocks to all the patches
    */
    void GeneratePatchData();

    /**
    * Places the assigned foliage on the patch
    * @param instance The instance ID to update
    */
    void PlaceFoliage(int instanceID);

    /**
    * Updates any data stored for the patch
    * @param instance The instance ID to update
    */
    void UpdatePatchData(int instanceID);

    /**
    * Determines the approximate height at the given coordinates
    * @param patchID The ID of the patch to update
    * @param x,z The coordinates to get the height at
    */
    float GetPatchHeight(int instanceID, float x, float z) const;

    /**
    * Key for obtaining the mesh instance assigned to a patch
    */
    struct MeshKey
    {
        int index = 0;       ///< Mesh ID
        int instance = 0;    ///< Mesh Instance ID
    };

    /**
    * Holds information on what meshes exist inside the patch
    */
    struct Patch
    {
        Float2 minBounds;            ///< Maximum global coordinates of the patch 
        Float2 maxBounds;            ///< Minimum global coordinates of the patch
        std::vector<MeshKey> rocks;     ///< Data for what rocks to use
        std::vector<MeshKey> foliage;   ///< Data for what foliage to use
    };

    SceneData& m_data;              ///< Data for manipulating the scene
    Terrain& m_sand;                ///< Main Sand terrain mesh
    Water& m_ocean;                 ///< Main Ocean terran mesh
    int m_patchPerRow = 0;          ///< The number of patches per row of the area
    int m_countRandom = 5;          ///< Instance count to vary for foliage
    float m_patchSize = 0.0f;       ///< The offset between sand/water patches
    std::vector<int> m_patches;     ///< The current ordering of the patches; holds the instance ID
    std::vector<int> m_previous;    ///< Buffer for reorganising the patches; holds the instance ID
    std::vector<Patch> m_patchData; ///< Holds patch data; key is the instance ID held in m_patches
    Int2 m_patchInside;             ///< The patch the camera is currently inside
};