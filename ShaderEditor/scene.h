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
    * @param tweakbar Used for runtime diagnostics
    */
    Scene(CTwBar* tweakbar);

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
    void InitialiseTweakBar();

    /**
    * Reloads the twear bar to show the desired selected parameters
    */
    void ReloadTweakBar();

    /**
    * Fills in mesh data from the given file
    * @param path The path for the mesh file
    * @param errorBuffer A string which is filled in upon errors
    * @param mesh The mesh object to load
    * @return whether creation was successful
    */
    bool CreateMesh(const std::string& path, std::string& errorBuffer, Mesh& mesh);

    std::unique_ptr<FragmentLinker> m_linker; ///< Linker for auto generating shaders

    std::vector<Shader> m_shaders; ///< All shaders in the scene
    std::vector<Mesh> m_alpha;     ///< All translucent meshes in the scene
    std::vector<Mesh> m_meshes;    ///< All meshes in the scene
    std::vector<Light> m_lights;   ///< All lights in the scene
    int m_selectedLight;           ///< Index of the selected light
    CTwBar* m_tweakbar;            ///< Used for runtime diagnostics
};                     