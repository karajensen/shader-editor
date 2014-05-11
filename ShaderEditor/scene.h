////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "elements.h"
#include "AntTweakBar.h"

/**
* Manager and owner of all objects and diagnostics
*/
class Scene
{
public:

    /**
    * Constructor
    */
    Scene();

    /**
    * Destructor
    */
    ~Scene();

    /**
    * Constructor
    * @return Whether the initialization was successful
    */
    bool Initialise();

    /**
    * Updates the scene
    */
    void Update();

    /**
    * Selects the next light to be editable
    */
    void SelectNextLight();

    /**
    * @return the meshes in the scene
    */
    const std::vector<Mesh>& GetMeshes() const;

    /**
    * @return the translucent meshes in the scene
    */
    const std::vector<Mesh>& GetAlpha() const;

    /**
    * @return the shaders in the scene
    */
    const std::vector<Shader>& GetShaders() const;

    /**
    * @return the lights in the scene
    */
    const std::vector<Light>& GetLights() const;

    /**
    * @return the textures in the scene
    */
    const std::vector<Texture>& GetTextures() const;

    /**
    * Adds tweakable scene parameters to the tweak bar
    * @param tweakbar Used for runtime diagnostics
    */
    void InitialiseTweakBar(CTwBar* tweakbar);

private:

    /**
    * Initialises the lighting for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseLighting();

    /**
    * Initialises the meshes for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseMeshes();

    /**
    * Adds tweakable scene parameters to the tweak bar
    */
    void InitialiseTweakParameters();

    /**
    * Removes tweakable scene parameters from the tweak bar
    */
    void ReleaseTweakParameters();

    /**
    * Adds a texture from a mesh if it doesn't already exist
    * @param name The name of the texture to add
    * @return The unique id of the texture added
    */
    int AddTexture(const std::string& name);

    /**
    * Fills in mesh data from the given file
    * @param path The path for the mesh file
    * @param errorBuffer A string which is filled in upon errors
    * @param mesh The mesh object to load
    * @return whether creation was successful
    */
    bool CreateMesh(const std::string& path, std::string& errorBuffer, Mesh& mesh);

    std::vector<Texture> m_textures;  ///< All textures in the scene
    std::vector<Shader> m_shaders;    ///< All shaders in the scene
    std::vector<Mesh> m_alpha;        ///< All translucent meshes in the scene
    std::vector<Mesh> m_meshes;       ///< All meshes in the scene
    std::vector<Light> m_lights;      ///< All lights in the scene
    int m_selectedLight;              ///< Index of the selected light
    TwType m_vectorType;              ///< Tweakable vector type for the tweak bar
    CTwBar* m_tweakbar;               ///< Used for runtime diagnostics
};                     