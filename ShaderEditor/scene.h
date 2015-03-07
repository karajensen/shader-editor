////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "boost/property_tree/ptree.hpp"
#include "sceneInterface.h"

class FragmentLinker;
class Diagnostic;

/**
* Manager and owner of all objects and diagnostics
*/
class Scene : public IScene
{
public:

    Scene();
    ~Scene();

    /**
    * Initialises the scene
    * @return Whether the initialization was successful
    */
    bool Initialise();

    /**
    * Ticks the scene
    * @param deltatime The time passed between ticks
    */
    void Tick(float deltatime);

    /**
    * @return the meshes in the scene
    */
    virtual const std::vector<Mesh>& Meshes() const override;

    /**
    * @return the water in the scene
    */
    virtual const std::vector<Water>& Waters() const override;

    /**
    * @return the shaders in the scene
    */
    virtual const std::vector<Shader>& Shaders() const override;

    /**
    * @return the lights in the scene
    */
    virtual const std::vector<Light>& Lights() const override;

    /**
    * @return the textures in the scene
    */
    virtual const std::vector<Texture>& Textures() const override;

    /**
    * @return the emitters in the scene
    */
    virtual const std::vector<Emitter>& Emitters() const override;

    /**
    * @return the post processing for the final image
    */
    virtual const PostProcessing& Post() const override;

    /**
    * @return the animating caustics texture
    */
    virtual const AnimatedTexture& Caustics() const override;

    /**
    * @return the names of the lights in the scene
    */
    std::vector<std::string> GetLightNames() const;

    /**
    * @return the names of the meshes in the scene
    */
    std::vector<std::string> GetMeshNames() const;

    /**
    * @return the names of the waters in the scene
    */
    std::vector<std::string> GetWaterNames() const;

    /**
    * @return the names of the emitters in the scene
    */
    std::vector<std::string> GetEmitterNames() const;

    /**
    * @return the names of the shaders in the scene
    */
    std::vector<std::string> GetShaderNames() const;

    /**
    * @return the names of the post maps in the scene
    */
    std::vector<std::string> GetPostMapNames() const;

    /**
    * @param index The index of the light to get
    * @return the light 
    */
    Light& GetLight(int index);

    /**
    * @param index The index of the mesh to get
    * @return the mesh 
    */
    Mesh& GetMesh(int index);

    /**
    * @param index The index of the water to get
    * @return the water 
    */
    Water& GetWater(int index);

    /**
    * @param index The index of the emitter to get
    * @return the emitter 
    */
    Emitter& GetEmitter(int index);

    /**
    * @param index The index of the shader to get
    * @return the shader 
    */
    Shader& GetShader(int index);

    /**
    * @return the post processing for the final image
    */
    PostProcessing& GetPost();

    /**
    * @return the diagnostics for the scene
    */
    Diagnostic& GetDiagnostics();

    /**
    * Sets which post map will currently be rendered
    * @param index The index for the map to render
    */
    void SetPostMap(int index);

    /**
    * Gets the name of the texture
    * @param index The index of the texture
    * @return the name of the texture
    */
    std::string GetTexture(int index);

    /**
    * @return the number of meshes available
    */
    int GetMeshCount() const;

    /**
    * @return the number of water available
    */
    int GetWaterCount() const;

    /**
    * @return the number of emitters available
    */
    int GetEmitterCount() const;

    /**
    * @return the number of lights available
    */
    int GetLightCount() const;

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
    */
    bool InitialiseDiagnostics();

    /**
    * Initialises any special textures not attached to a mesh
    */
    bool InitialiseTextures();

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
    * @param node The node for the mesh config file
    */
    void InitialiseMeshShader(Mesh& mesh,
                              FragmentLinker& linker,
                              const boost::property_tree::ptree& node);

    /**
    * Initialises any textures requires for the mesh
    * @param mesh The mesh to initialise
    */
    void InitialiseMeshTextures(MeshData& mesh);

    /**
    * Initialises a mesh for the scene
    * @param mesh The mesh to initialise
    * @param linker The fragment linker used to generate shaders
    * @param node The node for the mesh config file
    * @return if initialization was successfull
    */
    bool InitialiseMesh(Mesh& mesh, 
                        FragmentLinker& linker,
                        const boost::property_tree::ptree& node);

    /**
    * Initialises a water mesh for the scene
    * @param water The water to initialise
    * @param node The node for the mesh config file
    * @return if initialization was successfull
    */
    bool InitialiseWater(Water& water, const 
                         boost::property_tree::ptree& node);

    /**
    * Creates the mesh buffers
    * @param mesh The mesh to create
    * @param hasNormals Whether this mesh requires normals in the buffer
    */
    bool CreateMesh(MeshData& mesh, bool hasNormals);

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

    
    std::vector<Texture> m_textures;                  ///< All textures in the scene
    std::vector<Shader> m_shaders;                    ///< All shaders in the scene
    std::vector<Mesh> m_meshes;                       ///< All meshes in the scene
    std::vector<Light> m_lights;                      ///< All lights in the scene
    std::vector<Water> m_water;                       ///< All water in the scene
    std::vector<Emitter> m_emitters;                  ///< All particle emitters in the scene
    std::unique_ptr<Diagnostic> m_diagnostic;         ///< Diagnostics for the scene
    std::unique_ptr<PostProcessing> m_postProcessing; ///< Post processing for the final image
    std::unique_ptr<AnimatedTexture> m_caustics;      ///< Caustic animated texture
};                     