////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - elements.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "float3.h"
#include "matrix.h"
#include <array>
#include <vector>

static const int NO_INDEX = -1;

/**
* Vector with two floats
*/
struct Float2
{
    Float2() : x(0), y(0)
    {
    }

    Float2(float X, float Y) : x(X), y(Y)
    {
    }

    float x, y;
};

/**
* Colour with red, green, blue and alpha components
*/
struct Colour
{
    enum Component
    {
        RED,
        GREEN,
        BLUE,
        ALPHA
    };

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

/**
* GUI pages available
*/
enum GuiPage
{
    SCENE,
    LIGHT,
    MESH,
    POST,
    NO_PAGE
};

/**
* Holds data for post processing the final image
*/
struct PostProcessing
{
    /**
    * Post shader textures
    */
    enum Texture
    {
        SCENE,
        NORMAL,
        BLUR
    };

    /**
    * Post Shader maps
    */
    enum Map
    {
        FINAL_MAP,
        SCENE_MAP,
        NORMAL_MAP,
        DEPTH_MAP,
        GLOW_MAP,
        BLUR_SCENE_MAP,
        BLUR_GLOW_MAP,
        DOF_MAP,
        FOG_MAP,
        MAX_MAPS
    };

    /**
    * Constructor
    */
    PostProcessing()
    {
        SetPostMap(SCENE_MAP);

        weights[0] = 1.0f;
        weights[1] = 0.9f;
        weights[2] = 0.55f;
        weights[3] = 0.18f;
        weights[4] = 0.1f;
        NormaliseWeights();
    }

    /**
    * Normalises the blur weights
    */
    void NormaliseWeights()
    {
        const float overallWeight = weights[0] + 2.0f * 
            (weights[1] + weights[2] + weights[3] + weights[4]);

        weights[0] /= overallWeight;
        weights[1] /= overallWeight;
        weights[2] /= overallWeight;
        weights[3] /= overallWeight;
        weights[4] /= overallWeight;
    }

    /**
    * Sets which post map should be rendered
    * @param map The post map to render
    */
    void SetPostMap(Map map)
    {
        masks.assign(0.0f);
        masks[map] = 1.0f;
    }

    /**
    * @param map the map type to convert
    * @return the string name of the map type
    */
    static std::string GetMapName(Map map)
    {
        switch (map)
        {
        case FINAL_MAP:
            return "Final Scene";
        case SCENE_MAP:
            return "Scene Map";
        case NORMAL_MAP:
            return "Normal Map";
        case DEPTH_MAP:
            return "Depth Map";
        case GLOW_MAP:
            return "Glow Map";
        case BLUR_GLOW_MAP:
            return "Blur-Glow Map";
        case BLUR_SCENE_MAP:
            return "Blur-Scene Map";
        case DOF_MAP:
            return "DOF Map";
        case FOG_MAP:
            return "Fog Map";
        default:
            return "None";
        }
    }

    float dofDistance = 0.0f;            ///< Distance the depth of field starts
    float dofFade = 0.0f;                ///< How quick depth of field fades to the scene
    float glowAmount = 0.0f;             ///< The overall glow multiplier
    float contrast = 0.0f;               ///< Contrast controller of the final scene
    float saturation = 0.0f;             ///< Saturation controller of the final scene
    float blurStep = 0.0f;               ///< Sampling step for blurring
    float blurAmount = 0.0f;             ///< Amount to blur the scene by
    float depthNear = 0.0f;              ///< Value where depth colour is min
    float depthFar = 0.0f;               ///< Value where depth colour is max
    float fogDistance = 0.0f;            ///< Distance the fog starts
    float fogFade = 0.0f;                ///< How quick fog fades to the scene
    Colour fogColour;                    ///< Colour for the fog
    Colour minimumColour;                ///< Colour ranges for RGB
    Colour maximumColour;                ///< Colour ranges for RGB
    std::array<float, MAX_MAPS> masks;   ///< Visibility of post maps
    std::array<float, 5> weights;        ///< Normalised pixel weights for blurring
};

/**
* Texture rendered on a mesh
*/
struct Texture
{
    /**
    * Type of texture
    * Required in order of usage in shader body for OpenGL
    */
    enum Type
    {
        DIFFUSE,
        NORMAL,
        SPECULAR,
        GLOW,
        ENVIRONMENT,
        MAX_TYPES
    };

    /**
    * Gets a text description of the texture type
    * @param type The type to query for text
    * @return the text description of the type
    */
    static std::string GetTypeDescription(unsigned int type)
    {
        switch (type)
        {
        case DIFFUSE:
            return "Diffuse";
        case NORMAL:
            return "Normal";
        case SPECULAR:
            return "Specular";
        case GLOW:
            return "Glow";
        case ENVIRONMENT:
            return "Environment";
        default:
            return "None";
        };
    }

    std::string name; ///< Name of the texture
    std::string path; ///< Path to the texture
};

/**
* Shader for a mesh generated from components
*/
struct Shader
{
    /**
    * All shader components
    */
    enum Component
    {
        FLAT,
        BUMP,
        SPECULAR,
        GLOW,
        MAX_COMPONENTS
    };

    /**
    * Vertex Shader Attribute ordering
    */
    enum VertexAttribute
    {
        POSITION,
        UVS,
        NORMAL,
        COLOUR,
        TANGENT,
        BITANGENT
    };

    /**
    * Gets a text description of the shader component
    * @param component The component to query for text
    * @return the text description of the component
    */
    static std::string ComponentAsString(unsigned int component)
    {
        switch (component)
        {
        case FLAT:
            return "FLAT";
        case BUMP:
            return "BUMP";
        case SPECULAR:
            return "SPECULAR";
        case GLOW:
            return "GLOW";
        default:
            return "NONE";
        };
    }

    /**
    * Determines whether the shader has the component
    * @param component The component to query for text
    * @return whether the shader has the component
    */
    bool HasComponent(unsigned int component) const
    {
        return std::find(components.begin(), components.end(),
            Component(component)) != components.end();
    }

    int index = NO_INDEX;              ///< Unique index of the shader
    std::string name;                  ///< name of the shader
    std::string glslVertexFile;        ///< filename of the glsl shader
    std::string glslFragmentFile;      ///< filename of the glsl shader
    std::string hlslShaderFile;        ///< filename of the hlsl shader
    std::vector<Component> components; ///< Sections that make up this shader
};

/**
* Single light to provide illumination in the scene
*/
struct Light
{
    std::string name;          ///< Name of the light
    Colour diffuse;            ///< Colour of the light
    Colour specular;           ///< Specular highlights the light will cast
    Float3 attenuation;        ///< How much the light will fade in distance
    Float3 position;           ///< World coordinates of the light
	float specularity = 1.0f;  ///< Brightness of the specular highlights
};

/**
* Mesh object rendered in the scene
*/
struct Mesh
{
    Mesh()
    {
        textureIDs.resize(Texture::MAX_TYPES);
        textureIDs.assign(Texture::MAX_TYPES, NO_INDEX);
    }

    virtual ~Mesh() = default;

    float specularity = 1.0f;      ///< Brightness of the specular highlights
    float ambience = 1.0f;         ///< Ambient light multiplier
    float bump = 1.0f;             ///< Saturation of bump
    float glow = 1.0f;             ///< Intensity glow multiplier

    std::string name;              ///< Name of the mesh
    bool backfacecull = true;      ///< Whether back facing polygons are culled
    int shaderIndex = NO_INDEX;    ///< Unique Index of the mesh shader to use
    int normalIndex = NO_INDEX;    ///< Unique Index of the normal shader to use
    std::vector<float> vertices;   ///< Mesh Vertex information
    std::vector<DWORD> indices;    ///< Mesh Index information
    std::vector<int> textureIDs;   ///< IDs for each texture used
    int vertexComponentCount = 1;  ///< Number of components that make up a vertex
    int vertexCount = 0;           ///< Number of vertices in the mesh
    int faceCount = 0;             ///< Number of faces in the mesh
    int indexCount = 0;            ///< Number of indicies in the mesh
    int maxTextures = 0;           ///< Maximum textures used for this mesh
};

/**
* Data for a water wave
*/
struct Wave
{
    float frequency = 0.0f;         ///< Frequency of the wave
    float amplitude = 0.0f;         ///< Amplitude of the wave
    float speed = 0.0f;             ///< Speed of the wave
    Float2 direction;               ///< Direction the wave moves
};

/**
* Data for a water-shaded mesh
*/
struct Water : public Mesh
{
    float bumpSpeed = 0.0f;       ///< Speed of the bump movement
    float speed = 0.0f;           ///< Overally speed of the wave
    Float2 textureOffset;         ///< Offset of the water texture
    float fresnalFactor = 0.0f;   ///< Fresnal factor of water
    Colour shallowColour;         ///< Colour of the shallow water
    Colour deepColour;            ///< Colour of the deep water
    Colour reflectionTint;        ///< Colour of the reflection tint
    float reflection = 0.0f;      ///< Intensity of the reflections
    std::vector<Wave> waves;       ///< Wave infomation
};

/**
* Data for a single particle
*/
struct Particle
{
    Float3 direction;        ///< The direction of the particle
    Float3 position;         ///< The position of the particle
    float speed = 0.0f;      ///< The speed of the particle
    int texture = NO_INDEX;  ///< The texture to render the particle with
    bool alive = false;      ///< Whether this particle should be rendered
};

/**
* Data for a particle emitter
*/
struct Emitter
{
    float width = 0.0f;              ///< The width of the emitter
    float length = 0.0f;             ///< The length of the emitter
    float speed = 0.0f;              ///< The speed of the particles
    float speedVariation = 0.0f;     ///< The variation in speed of the particles
    float lifeTime = 0.0f;           ///< Seconds the particle can live before dying
    Float3 position;                 ///< The position of the emitter
    Float3 direction;                ///< The direction the particles will spawn 
    std::vector<int> textures;       ///< Indexes for the particle textures to use
    std::vector<Particle> particles; ///< Particles this emitter can spawn
    int shaderIndex = NO_INDEX;      ///< Unique Index of the mesh shader to use
    int normalIndex = NO_INDEX;      ///< Unique Index of the normal shader to use
    std::string name;                ///< Name of this emitter
};