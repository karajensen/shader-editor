////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneData.h
////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "shader.h"
#include "mesh.h"
#include "water.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"
#include "texture.h"
#include "textureAnimated.h"
#include "textureProcedural.h"
#include "postprocessing.h"
#include "diagnostic.h"

/**
* Internal data for the scene
*/
struct SceneData
{
    std::vector<std::unique_ptr<Shader>> shaders;    ///< All shaders in the scene
    std::vector<std::unique_ptr<Mesh>> meshes;       ///< All meshes in the scene
    std::vector<std::unique_ptr<Light>> lights;      ///< All lights in the scene
    std::vector<std::unique_ptr<Texture>> textures;  ///< All textures in the scene
    std::vector<std::unique_ptr<Terrain>> terrain;   ///< All terrain in the scene
    std::vector<std::unique_ptr<Water>> water;       ///< All water in the scene
    std::vector<std::unique_ptr<Emitter>> emitters;  ///< All emitters in the scene
    std::unique_ptr<AnimatedTexture> caustics;       ///< Caustic animated texture
    std::unique_ptr<Diagnostic> diagnostics;         ///< Scene diagnostics
    std::unique_ptr<PostProcessing> post;            ///< Data for post processing
    std::vector<unsigned int> proceduralTextures;    ///< Indices of all editable textures
    unsigned int sandIndex = 0;                      ///< Index for the sand terrain
    unsigned int oceanIndex = 0;                     ///< Index for the ocean water
};