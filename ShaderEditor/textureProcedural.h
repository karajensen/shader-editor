////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "texture.h"

struct Cache;

/**
* Manages generating textures 
*/
class ProceduralTexture : public Texture
{
public:

    /**
    * Avaliable types
    */
    enum Type
    {
        RANDOM
    };

    /**
    * Constructor
    * @param name The filename of the texture
    * @param path The full path to the texture
    * @param size The dimensions of the texture
    * @param type The type of texture to make
    * @param filter The filtering to use
    */
    ProceduralTexture(const std::string& name, 
                      const std::string& path,
                      int size,
                      Type type,
                      Filter filter);

    /**
    * Writes to the data in the cache
    * @param cache The cache of data from the GUI
    */
    void Write(Cache& cache);

    /**
    * Reads the data from the cache
    * @param cache The cache of data from the GUI
    */
    void Read(Cache& cache);

    /**
    * @return the pixels of the texture or nullptr if empty
    */
    virtual const unsigned int* Pixels() const override;

    /**
    * @return the size of the texture if set
    */
    virtual int Size() const override;

    /**
    * @return whether this texture has explicitly set pixels
    */
    virtual bool HasPixels() const override;

    /**
    * Saves the texture to file
    */
    void SaveTexture();

private:

    /**
    * Creates a texture of random normals used for ambient occlusion
    */
    void MakeRandomNormals();

    Type m_type;                        ///< The type of texture this is
    std::vector<unsigned int> m_pixels; ///< Pixels of the texture
    int m_size;                         ///< Dimensions of the texture
    std::string m_savePath;             ///< Path the generated texture is saved to

};