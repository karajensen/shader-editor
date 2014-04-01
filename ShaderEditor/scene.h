////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "elements.h"

struct CTwBar;
class FragmentLinker;

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
    const std::vector<Mesh> GetMeshes() const;

    /**
    * @return the translucent meshes in the scene
    */
    const std::vector<Mesh> GetAlpha() const;

    /**
    * @return the shaders in the scene
    */
    const std::vector<Shader> GetShaders() const;

    /**
    * @return the lights in the scene
    */
    const std::vector<Light> GetLights() const;

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
    * Fills in mesh data from the given file
    * @param path The path for the mesh file
    * @param errorBuffer A string which is filled in upon errors
    * @param mesh The mesh object to load
    * @return whether creation was successful
    */
    bool CreateMesh(const std::string& path, std::string& errorBuffer, Mesh& mesh);

    std::unique_ptr<FragmentLinker> m_linker; ///< Linker for auto generating shaders
    std::vector<Shader> m_shaders;            ///< All shaders in the scene
    std::vector<Mesh> m_alpha;                ///< All translucent meshes in the scene
    std::vector<Mesh> m_meshes;               ///< All meshes in the scene
    std::vector<Light> m_lights;              ///< All lights in the scene
    int m_selectedLight;                      ///< Index of the selected light
    CTwBar* m_tweakbar;                       ///< Used for runtime diagnostics
};                     