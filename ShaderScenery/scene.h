////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <boost/noncopyable.hpp>
#include "common.h"

class Mesh;
class ShaderEditor;
class GeneratedShader;
class PostShader;
class NormalShader;
class Quad;
class TextureManager;

/**
* Class for managing a scene
*/
class Scene : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit Scene(EnginePtr engine);

    /**
    * Destructor
    */
    ~Scene();

    /**
    * Renders the scene
    */
    void Render();

    /**
    * Initialise the scene
    * @return whether or not initialisation succeeded
    */
    bool Initialise();

    /**
    * Reload meshes from file
    */
    void ReloadMeshesFromFile();

    /**
    * Toggles whether meshes are rendered as wireframe
    */
    void ToggleWireframe();

    /**
    * @return the shader for post processing
    */
    boost::shared_ptr<PostShader> GetPostShader();

    /**
    * @return the editor for generated shaders
    */
    boost::shared_ptr<ShaderEditor> GetShaderEditor();

private:

    /**
    * Loads in mesh data from a boost property tree
    * @return whether or not all mesh creation succeeded
    */
    bool CreateMeshes();

    /**
    * Creates all render targets and billboards for them to render on
    * @return whether creation succeeded
    */
    bool CreateRenderTargets();

    /**
    * Creates a render target given a irrlicht texture object
    * @param rendertarget A pointer to the ITexture pointer
    * @param name The name of the render target
    * @param size The size of the render target
    * @return whether creation succeeded
    */
    bool CreateRenderTarget(ITexture** rendertarget, char* name, int size);

    typedef boost::shared_ptr<PostShader> Post_Ptr;
    typedef boost::shared_ptr<GeneratedShader> Shader_Ptr;
    typedef boost::shared_ptr<Mesh> Mesh_Ptr;
    typedef std::vector<Mesh_Ptr> Mesh_Container;
    typedef std::vector<Shader_Ptr> Shader_Container;
    typedef std::vector<Post_Ptr> Post_Container;

    EnginePtr m_engine;          ///< Irrlicht engine
    Mesh_Container m_meshes;     ///< Array of pointers to meshes
    Shader_Container m_shaders;  ///< Array of pointers to shaders
    ITexture* m_diffuseTarget;   ///< Texture storing diffuse lighting info for scene
    ITexture* m_normalTarget;    ///< RGB channels = normal info, A channel = depth info
    int m_renderTargetSize;      ///< Standard size for each render target
    int m_normalShaderIndex;     ///< Index for the normal shader index
    SColor m_drawColour;         ///< Colour to refresh the back buffer with
    bool m_wireframe;            ///< Whether meshes are rendered as wireframe

    boost::scoped_ptr<TextureManager> m_texture;     ///< Texture mananger
    boost::scoped_ptr<Quad> m_quad;                  ///< Post processing screen quad
    boost::shared_ptr<ShaderEditor> m_editor;        ///< Shader editor for toggling components
    boost::shared_ptr<PostShader> m_postShader;      ///< Post processing shader
    boost::shared_ptr<NormalShader> m_normalShader;  ///< Normal-depth shader
};