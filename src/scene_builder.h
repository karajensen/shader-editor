////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene_builder.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "texture_procedural.h"

class Mesh;
class Light;
class Shader;
class Terrain;
class MeshData;
class FragmentLinker;
struct EmitterData;
struct SceneData;

/**
* Builds all objects and diagnostics for the scene
*/
class SceneBuilder
{
public:

    /**
    * Constructor
    * @param data All information for the scene
    */
    SceneBuilder(SceneData& data);

    /**
    * Destructor
    */
    ~SceneBuilder();

    /**
    * Initialises the scene
    * @return Whether the initialization was successful
    */
    bool Initialise();

private:

    /**
    * Prevent copying
    */
    SceneBuilder(const SceneBuilder&) = delete;
    SceneBuilder& operator=(const SceneBuilder&) = delete;

    /**
    * Initiliases the diagnostics
    * @return Whether the initialization was successful
    */
    bool InitialiseDiagnostics();

    /**
    * Initiliases all shaders
    * @return Whether the initialization was successful
    */
    bool InitialiseShaders();

    /**
    * Initialises the lighting for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseLighting();

    /**
    * Initialises all textures required
    * @return Whether the initialization was successful
    */
    bool InitialiseTextures();

    /**
    * Initialises the meshes for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseMeshes();

    /**
    * Initialises the shadows for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseShadows();

    /**
    * Initialises the water for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseWater();

    /**
    * Initialises the emitters for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseEmitters();

    /**
    * Initialises the terrain for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseTerrain();

    /**
    * Initialises a mesh
    * @param name The name of the mesh
    * @param filename The filename of the mesh
    * @param uvScale The scale for the UVs
    * @param shaderName The shader to use
    * @return The mesh initialised
    */
    Mesh& InitialiseMesh(const std::string& name,
                         const std::string& filename,
                         float uScale,
                         float vScale,
                         const std::string& shaderName);

    /**
    * Initialises terrain
    * @param name The name of the mesh
    * @param heightmap The name of the height map
    * @param shaderName The shader to use
    * @param uvTextureStretch The texture stretch multipliers
    * @param tiling whether to force edges to match
    * @param height The height the terrain is rendered at
    * @param minHeight The minimum height offset of the terrain
    * @param maxHeight The maximum height offset of the terrain
    * @param spacing The spacing between vertices
    * @param size The number of vertices
    * @return The terrain initialised
    */
    Terrain& InitialiseTerrain(const std::string& name,
                               const std::string& heightmap,
                               const std::string& shaderName,
                               float uvTextureStretch,
                               bool tiling,
                               float height,
                               float minHeight,
                               float maxHeight,
                               float spacing,
                               int size);
    /**
    * Initialises a shader
    * @param linker the generator for shaders
    * @param name the filename of the shader
    * @param components What the shader is made up of
    * @param index The index to add the shader at
    * @return Whether the initialization was successful
    */
    bool InitialiseShader(FragmentLinker& linker,
                          const std::string& name, 
                          unsigned int components,
                          int index = -1);

    /**
    * Initialises the caustics animation
    * @return Whether the initialization was successful
    */
    bool InitialiseCaustics();

    /**
    * Initialises the bubble emitter
    * @return Whether the initialization was successful 
    */
    bool InitialiseBubbles();

    /**
    * Initialises a texture
    * @param name The name of the texture
    * @param path The path of the texture
    * @param type The image type of the texture
    * @param filter The type of filtering to use
    * @return Whether the initialization was successful
    */
    bool InitialiseTexture(const std::string& name, 
                           const std::string& path,
                           Texture::Type type,
                           Texture::Filter filter = Texture::LINEAR);

    /**
    * Initialises a procedural texture
    * @param name The name of the texture
    * @param generation The type of procedural algorithm to use
    * @param size The size of the texture
    * @return whether initialisation succeeded
    */
    bool InitialiseTexture(const std::string& name, 
                           ProceduralTexture::Generation generation,
                           int size);

    /**
    * Initialises an emitter
    * @param name The name of the emitter
    * @param shader The id of the shader to use
    * @param textures The particle textures to use
    * @param data The data for the emission
    */
    bool InitialiseEmitter(const std::string& name, 
                           const std::string& shaderName,
                           const std::vector<int>& textures,
                           const EmitterData& data);

    /**
    * Creates new foliage mesh group(s) for the meshes
    * @param meshes The types of meshes to add to the group
    * @param hasShadow Whether this mesh requires a shadow
    * @param instances The number of groups to create
    * @return whether creation was successful
    */
    bool AddFoliage(std::initializer_list<const MeshData*> meshes,
                    bool hasShadow,
                    int instances);

    SceneData& m_data; ///< The scene to build
};                     