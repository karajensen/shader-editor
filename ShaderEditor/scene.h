////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "sceneElements.h"
class FragmentLinker;

/**
* Manager and owner of all objects and diagnostics
*/
class Scene : public SceneElements
{
public:

    /**
    * Constructor
    * @return Whether the initialization was successful
    */
    bool Initialise();

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
    * @param index The index of the shader to get
    * @return the shader 
    */
    Shader& GetShader(int index);

    /**
    * @return the post processing for the final image
    */
    PostProcessing& GetPost();

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
    * @return the number of lights available
    */
    int GetLightCount() const;

    /**
    * Outputs the meshes and emitters to an xml file
    */
    void SaveMeshesToFile();

    /**
    * Outputs the lights to an xml file
    */
    void SaveLightsToFile();

    /**
    * Outputs post processing to an xml file
    */
    void SavePostProcessingtoFile();

private:

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
    * Initialises the meshes for the scene
    * @param linker The fragment linker used to generate shaders
    * @return Whether the initialization was successful
    */
    bool InitialiseMeshes(FragmentLinker& linker);

    /**
    * Initialises a mesh shader for the scene
    * @param mesh The mesh to initialise
    * @param linker The fragment linker used to generate shaders
    * @param it The iterator for the mesh config file
    */
    void InitialiseMeshShader(Mesh& mesh,
                              FragmentLinker& linker,
                              boost::property_tree::ptree::iterator& it);

    /**
    * Initialises a mesh for the scene
    * @param mesh The mesh to initialise
    * @param it The iterator for the mesh config file
    */
    void InitialiseMesh(Mesh& mesh, boost::property_tree::ptree::iterator& it);

    /**
    * Initialises a water mesh for the scene
    * @param water The water to initialise
    * @param it The iterator for the mesh config file
    */
    void InitialiseWater(Water& water, boost::property_tree::ptree::iterator& it);

    /**
    * Initialises an emitter for the scene
    * @param emitter The emitter to initialise
    * @param it The iterator for the mesh config file
    */
    void InitialiseEmitter(Emitter& emitter, boost::property_tree::ptree::iterator& it);

    /**
    * Adds a texture from a mesh if it doesn't already exist
    * @param name The name of the texture to add
    * @return The unique id of the texture added
    */
    int AddTexture(const std::string& name);

    /**
    * Fills in mesh data
    * @param mesh The mesh object to load
    * @return whether creation was successful
    */
    bool CreateMesh(Mesh& mesh);

    /**
    * Fills in mesh data from the given file
    * @param path The path for the mesh file
    * @param errorBuffer A string which is filled in upon errors
    * @param mesh The mesh object to load
    * @return whether creation was successful
    */
    bool CreateMesh(const std::string& path, 
                    std::string& errorBuffer,  
                    Mesh& mesh);

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
    * Adds the given mesh to the given tree
    * @param mesh The mesh data to add
    * @param entry The tree to add to
    */
    void AddMeshToTree(const Mesh& mesh, 
                       boost::property_tree::ptree& entry);

    /**
    * Adds the given water to the given tree
    * @param water The water data to add
    * @param entries All entries for the tree
    * @param entry The tree to add to
    */
    void AddWaterToTree(const Water& water, 
                        std::vector<boost::property_tree::ptree>& entries,
                        boost::property_tree::ptree& entry);

    /**
    * Adds the given emitter to the given tree
    * @param emitter The emitter data to add
    * @param entry The tree to add to
    */
    void AddEmitterToTree(const Emitter& mesh, 
                          boost::property_tree::ptree& entry);
                            
    PostProcessing m_postProcessing;  ///< Post processing for the final image
    std::vector<Texture> m_textures;  ///< All textures in the scene
    std::vector<Shader> m_shaders;    ///< All shaders in the scene
    std::vector<Mesh> m_meshes;       ///< All meshes in the scene
    std::vector<Light> m_lights;      ///< All lights in the scene
    std::vector<Water> m_water;       ///< All water in the scene
    std::vector<Emitter> m_emitters;  ///< All particle emitters in the scene
};                     