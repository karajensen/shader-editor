////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scenePlacer.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "meshgroup.h"
#include "float3.h"
#include "int2.h"

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
    * @param cameraPosition The position of the camera
    * @return whether initialisation was successful
    */
    bool Initialise(const Float3& cameraPosition);

    /**
    * Updates the scene
    * @param cameraPosition The position of the camera
    */
    void Update(const Float3& cameraPosition);

    /**
    * Resets the assets held by each patch
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
    * @return whether assignment was successful
    */
    bool GeneratePatchData();

    /**
    * Updates any meshes stored for the patch
    * @param row/column The position of the patch in the grid
    */
    void UpdatePatchMeshes(int row, int column);

    /**
    * Updates any data stored for the patch
    * @param row/column The position of the patch in the grid
    */
    void UpdatePatchData(int row, int column);

    /**
    * Places the assigned foliage on the patch
    * @param instance The instance ID to update
    */
    void PlaceFoliage(int instanceID);

    /**
    * Places the assigned emitters on the patch
    * @param instance The instance ID to update
    */
    void PlaceEmitters(int instanceID);

    /**
    * Places the assigned rock on the patch
    * @param instance The instance ID to update
    */
    void PlaceRock(int instanceID);

    /**
    * Determines the approximate height at the given coordinates
    * @param patchID The ID of the patch to use
    * @param x/z The position to get the height at
    */
    Float3 GetPatchLocation(int patchID, float x, float z);

    /**
    * Resets all assigned foliage and emitters for a patch
    */
    void ResetFoliage();

    /**
    * Holds information on what meshes exist inside the patch
    */
    struct Patch
    {
        bool requiresUpdate = false;        ///< Flag for updating the foliage/assets
        Int2 coordinates;             ///< Coordinates of the patch within the grid
        std::vector<MeshGroup> foliage;     ///< Data for what foliage to use
        std::vector<InstanceKey> emitters;  ///< Data for what emitters to use
        InstanceKey rock;                   ///< Single assigned rock terrain
    };
                                      
    SceneData& m_data;                ///< Data for manipulating the scene
    Terrain& m_sand;                  ///< Main Sand terrain mesh
    Water& m_ocean;                   ///< Main Ocean terran mesh
    int m_patchPerRow = 0;            ///< The number of patches per row of the area
    float m_meshMinScale = 0.0f;      ///< Random mesh scaling amount
    float m_meshMaxScale = 0.0f;      ///< Random mesh scaling amount
    float m_patchSize = 0.0f;         ///< The offset between sand/water patches
    float m_rockOffset = 0.0f;        ///< Random rock offset from the patch center
    Float3 m_rockMinScale;            ///< Random rock scaling amount
    Float3 m_rockMaxScale;            ///< Random rock scaling amount
    float m_shadowOffset = 0.2f;      ///< Height above terrain to place shadows
    float m_shadowScale = 4.0f;       ///< Scale of shadows in comparison to mesh
    std::vector<int> m_patches;       ///< The current ordering of the patches; holds the instance ID
    std::vector<int> m_previous;      ///< Buffer for reorganising the patches; holds the instance ID
    std::vector<Patch> m_patchData;   ///< Holds patch data; key is the instance ID held in m_patches
    Int2 m_patchInside;               ///< The patch the camera is currently inside
};