////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "texture.h"

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
        RANDOM,
        DIAMOND_SQUARE
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
    virtual void Write(Cache& cache) override;

    /**
    * Reads the data from the cache
    * @param cache The cache of data from the GUI
    */
    virtual void Read(Cache& cache) override;

    /**
    * @return the pixels of the texture or nullptr if empty
    */
    virtual const std::vector<unsigned int>& Pixels() const override;

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
    virtual void Save() override;

    /**
    * Reloads the texture
    */
    virtual void Reload() override;

private:

    /**
    * Creates a texture of random normals used for ambient occlusion
    */
    void MakeRandomNormals();

    /**
    * Creates a fractal texture using the diamond square algorithm
    */
    void MakeDiamondSquareFractal();

    /**
    * @return the colour from the row and column value
    */
    unsigned int Get(int row, int column) const;

    /**
    * @return the index from the row and column value
    */
    unsigned int Index(int row, int column) const;

    /**
    * Sets the colour from each component
    */
    void Set(unsigned int index, int r, int g, int b, int a);

    /**
    * Sets the colour from each component
    */
    void Set(unsigned int index, float r, float g, float b, float a);

    /**
    * Sets the colour as a float
    */
    void Set(int row, int column, float value);

    /**
    * Sets the colour component as an int
    */
    void SetRed(unsigned int index, int value);
    void SetGreen(unsigned int index, int value);
    void SetBlue(unsigned int index, int value);
    void SetAlpha(unsigned int index, int value);

    /**
    * Sets the colour component as a float
    */
    void SetRed(unsigned int index, float value);
    void SetGreen(unsigned int index, float value);
    void SetBlue(unsigned int index, float value);
    void SetAlpha(unsigned int index, float value);

    /**
    * Gets the colour component as a char
    */
    unsigned char RedAsChar(unsigned int index);
    unsigned char GreenAsChar(unsigned int index);
    unsigned char BlueAsChar(unsigned int index);
    unsigned char AlphaAsChar(unsigned int index);

    /**
    * Gets the colour component as an int
    */
    int RedAsInt(unsigned int index);
    int GreenAsInt(unsigned int index);
    int BlueAsInt(unsigned int index);
    int AlphaAsInt(unsigned int index);

    /**
    * Gets the colour component as a float
    */
    float RedAsFlt(unsigned int index);
    float GreenAsFlt(unsigned int index);
    float BlueAsFlt(unsigned int index);
    float AlphaAsFlt(unsigned int index);

    Type m_type;                        ///< The type of texture this is
    std::vector<unsigned int> m_pixels; ///< Pixels of the texture
    int m_size;                         ///< Dimensions of the texture
    std::string m_savePath;             ///< Path the generated texture is saved to
};