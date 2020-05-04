////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene_data.h
////////////////////////////////////////////////////////////////////////////////////////

#include "shader.h"
#include "mesh.h"
#include "water.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"
#include "texture.h"
#include "animation.h"
#include "texture_procedural.h"
#include "postprocessing.h"
#include "diagnostic.h"
#include "mesh_group.h"

/**
* Internal data for the scene
*/
struct SceneData
{
    /**
    * Constructor
    */
    SceneData() :
        post(std::make_unique<PostProcessing>())
    {
    }

    std::vector<std::unique_ptr<Shader>> shaders;      ///< All shaders in the scene
    std::vector<std::unique_ptr<Mesh>> meshes;         ///< All meshes in the scene
    std::vector<std::unique_ptr<Light>> lights;        ///< All lights in the scene
    std::vector<std::unique_ptr<Texture>> textures;    ///< All textures in the scene
    std::vector<std::unique_ptr<Terrain>> terrain;     ///< All terrain in the scene
    std::vector<std::unique_ptr<Water>> water;         ///< All water in the scene
    std::vector<std::unique_ptr<Emitter>> emitters;    ///< All emitters in the scene
    std::unique_ptr<Animation> caustics;               ///< Caustic animated texture
    std::unique_ptr<Diagnostic> diagnostics;           ///< Scene diagnostics
    std::unique_ptr<PostProcessing> post;              ///< Data for post processing
    std::unique_ptr<MeshData> shadows;                 ///< Shadow instances
    std::vector<unsigned int> proceduralTextures;      ///< Indices of all editable textures
    std::vector<MeshGroup> foliage;                    ///< Available foliage for placing in scene
    std::vector<InstanceKey> rocks;                    ///< Avaliable rocks for placing in scene
    unsigned int sandIndex = 0;                        ///< Index for the sand terrain
    unsigned int oceanIndex = 0;                       ///< Index for the ocean water
    unsigned int sunIndex = 0;                         ///< Index for the initial sun light
};