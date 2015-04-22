////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "boost/property_tree/ptree.hpp"

struct SceneData;
class FragmentLinker;
class MeshData;

/**
* Builds all objects and diagnostics for the scene
*/
class SceneBuilder
{
public:

    /**
    * Constructor
    * @param data Data for manipulating the scene
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

    /**
    * Outputs the scene to an xml file
    */
    void SaveSceneToFile();

private:

    /**
    * Outputs post processing to an xml file
    */
    void SavePostProcessingtoFile();

    /**
    * Outputs the meshes and emitters to an xml file
    */
    void SaveMeshesToFile();

    /**
    * Outputs the lights to an xml file
    */
    void SaveLightsToFile();

    /**
    * Outputs particle emitters to an xml file
    */
    void SaveParticlesToFile();

    /**
    * Initiliases any stand-alone and shared shaders explicitly
    * @param linker The fragment linker used to generate shaders
    * @return Whether the initialization was successful
    */
    bool InitialiseShaders(FragmentLinker& linker);

    /**
    * Initialises the post processing for the final image
    * @return Whether the initialization was successful
    */
    bool InitialisePost();

    /**
    * Initialises the lighting for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseLighting();

    /**
    * Initialises the emitters for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseEmitters();

    /**
    * Initialises the diagnostics in the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseDiagnostics();

    /**
    * Initialises all textures required
    * @return Whether the initialization was successful
    */
    bool InitialiseTextures();

    /**
    * Initialises the caustics textures
    * @return Whether the initialization was successful
    */
    bool InitialiseCaustics();

    /**
    * Initialises the meshes for the scene
    * @param linker The fragment linker used to generate shaders
    * @return Whether the initialization was successful
    */
    bool InitialiseMeshes(FragmentLinker& linker);

    /**
    * Initialises a mesh shader for the scene
    * @param mesh The mesh to initialise
    * @param linker The fragment linker used to generate shaders
    */
    void InitialiseMeshShader(MeshData& mesh, FragmentLinker& linker);

    /**
    * Initialises any textures requires for the mesh
    * @param mesh The mesh to initialise
    */
    void InitialiseMeshTextures(MeshData& mesh);

    /**
    * Initialises a mesh for the scene
    * @param node The mesh data to initialise
    * @param linker The fragment linker used to generate shaders
    * @param isFoliage whether this mesh is considered foliage in the scene
    * @return if initialization was successfull
    */
    bool InitialiseMesh(const boost::property_tree::ptree& node, 
                        FragmentLinker& linker, 
                        bool isFoliage);

    /**
    * Initialises terrain for the scene
    * @param node The terrain data to initialise
    * @param linker The fragment linker used to generate shaders
    * @return if initialization was successfull
    */
    bool InitialiseTerrain(const boost::property_tree::ptree& node, 
                           FragmentLinker& linker);

    /**
    * Initialises a water mesh for the scene
    * @param node The water data to initialise
    * @return if initialization was successfull
    */
    bool InitialiseWater(const boost::property_tree::ptree& node);

    /**
    * Adds a texture from a mesh if it doesn't already exist
    * @param name The name of the texture to add
    * @return The unique id of the texture added
    */
    int AddTexture(const std::string& name);

    /**
    * Gets the index for the shader if possible
    * @param shadername The name of the shader
    * @return the index for the shader or -1 if can't find
    */
    int GetShaderIndex(const std::string& shadername);

    /**
    * Gets the index for the shader
    * @param linker The fragment linker
    * @param shaderName The name of the shader to get the index for
    * @param meshName The name ofthe mesh the shader will be used for
    * @return the index for the shader
    */
    int GetShaderIndex(FragmentLinker& linker, 
                       const std::string& shadername, 
                       const std::string& meshName);

    /**
    * @return whether the mesh at the given index is considered foliage
    */
    bool IsFoliage(unsigned int index) const;

    SceneData& m_data; ///< The scene to build
};                     