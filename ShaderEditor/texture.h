////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>

/**
* Texture rendered on a mesh
*/
class Texture
{
public:

    /**
    * Type of texture
    * Ordering must much usage in shader body
    */
    enum Type
    {
        DIFFUSE,
        NORMAL,
        SPECULAR,
        ENVIRONMENT,
        CAUSTICS,
        MAX_TYPES
    };

    /**
    * Constructor
    * @param name The filename of the texture
    * @param path The full path to the texture
    */
    Texture(const std::string& name, const std::string& path);

    /**
    * @return the filename of the texture
    */
    const std::string& Name() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Path() const;

    /**
    * Gets a text description of the texture type
    * @param type The type to query for text
    * @return the text description of the type
    */
    static std::string GetTypeDescription(unsigned int type);

    /**
    * Determines whether this texture type is animated
    * @param type The type to query
    * @return whether it is animated
    */
    static bool IsAnimatedType(unsigned int type);

private:

    std::string m_name; ///< Name of the texture
    std::string m_path; ///< Path to the texture
};

/**
* Manages cycling through multiple textures
*/
class AnimatedTexture
{
public:

    /**
    * Adds a texture frame 
    * @param ID the unique ID of the texture to add
    */
    void AddFrame(int ID);

    /**
    * @return the current frame this texture is in
    */
    int GetFrame() const;

    /**
    * Ticks the animated texture
    */
    void Tick(float deltatime);

private:

    std::vector<int> m_frames; ///< All frames to animate
    int m_selectedFrame = 0;   ///< The currently selected frame
    float m_timePassed = 0.0f; ///< The time passed since last frame switch

};