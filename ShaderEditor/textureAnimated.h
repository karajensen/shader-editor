////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureAnimated.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

/**
* Manages cycling through multiple textures
*/
class AnimatedTexture
{
public:

    /**
    * Constructor
    * @param path The path where the frames are
    * @param name The generic name of a frame
    * @param extension The file extension of a frame
    */
    AnimatedTexture(const std::string& path, 
                    const std::string& name,
                    const std::string& extension);

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
    * @return all the paths for the frames
    */
    const std::vector<std::string>& Paths() const;

    /**
    * Ticks the animated texture
    */
    void Tick(float deltatime);

    /**
    * Sets the speed of the animation
    */
    void SetSpeed(float speed);

    /**
    * @return the speed of the animation
    */
    float GetSpeed() const;

private:

    std::vector<std::string> m_paths; ///< Paths to all the frames
    std::vector<int> m_frames;        ///< All frames to animate
    int m_selectedFrame = 0;          ///< The currently selected frame
    float m_timePassed = 0.0f;        ///< The time passed since last frame switch
    float m_speed = 0.03f;            ///< The speed of the animation  

};