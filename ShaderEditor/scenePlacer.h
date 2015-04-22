////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scenePlacer.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "float3.h"
#include "int2.h"
#include "boost/noncopyable.hpp"
#include <vector>

struct SceneData;

/**
* Updates the scene depending on the camera position
* Holds information about the area which consists of patches of tiled water/sand
* Assumes water and sand patches are aligned and the same size
*/
class ScenePlacer : boost::noncopyable
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

private:

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
    * @return whether the index is a valid patch
    */
    bool IsValid(int index) const;

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
    void GenerateFoliage();

    /**
    * Places the assigned foliage on the patch
    * @param patchID The ID of the patch to update
    */
    void PlaceFoliage(int ID);

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
        std::vector<MeshKey> rocks;     ///< Data for what rocks to use
        std::vector<MeshKey> foliage;   ///< Data for what foliage to use
    };

    SceneData& m_data;              ///< Data for manipulating the scene
    int m_patchPerRow = 0;          ///< The number of patches per row of the area
    int m_countRandom = 5;          ///< Instance count to vary for foliage
    float m_patchSize = 0.0f;       ///< The offset between sand/water patches
    std::vector<int> m_patches;     ///< The current ordering of the patches
    std::vector<int> m_previous;    ///< Buffer for reorganising the patches
    std::vector<Patch> m_patchData; ///< Holds information about what is in a patch
    Int2 m_patchInside;       ///< The patch the camera is currently inside
};