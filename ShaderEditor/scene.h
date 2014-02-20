////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

struct CTwBar;
class FragmentLinker;

/**
* Individual shader object
*/
struct Shader
{
    /**
    * Constructor
    */
    Shader();

    int index;        ///< Unique index of the shader
    std::string name; ///< name of the shader
    std::string file; ///< filename of the shader
};

/**
* Individual light object
*/
struct Light
{
    /**
    * Constructor
    */
    Light();

    bool castshadow;     ///< Whether the light casts shadows or not
    std::string name;    ///< Name of the light
    Colour diffuse;      ///< Colour of the light
    Colour specular;     ///< Specular highlights the light will cast
    Float3 attenuation;  ///< How much the light will fade in distance
    Float3 position;     ///< World coordinates of the light
    float specularity;   ///< Brightness of the specular highlights
};

/**
* Mesh Vertex structure
*/
struct Vertex
{
    /**
    * Contructor
    */
    Vertex();

    float x,y,z;     ///< Vertex position
    float nx,ny,nz;  ///< Vertex normal
    float u,v;       ///< Vertex uvs
};

/**
* Inidividual mesh object
*/
struct Mesh
{
    /**
    * Constructor
    */
    Mesh();

    bool backfacecull;              ///< Whether back facing polygons are culled
    int shaderIndex;                ///< Index of which shader to use
    std::string name;               ///< Name of the mesh
    float specularity;              ///< Brightness of the specular highlights
    std::vector<Vertex> vertices;   ///< Vertex buffer
    std::vector<DWORD> indices;     ///< Index buffer
};

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
    * Fills in mesh data from the given file
    * @param path The path for the mesh file
    * @param errorBuffer A string which is filled in upon errors
    * @param mesh The mesh object to load
    * @return whether creation was successful
    */
    bool CreateMesh(const std::string& path, std::string& errorBuffer, Mesh& mesh);

    std::unique_ptr<FragmentLinker> m_linker; ///< Linker for auto generating shaders

    std::vector<Shader> m_shaders; ///< All shaders in the scene
    std::vector<Mesh> m_meshes;    ///< All meshes in the scene
    std::vector<Light> m_lights;   ///< All lights in the scene
    int m_selectedLight;           ///< Index of the selected light
    CTwBar* m_tweakbar;            ///< Used for runtime diagnostics
};                     