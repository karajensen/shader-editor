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
    * Constructor
    * @param name The filename of the texture
    * @param path The full path to the texture
    * @param size The dimensions of the texture
    */
    ProceduralTexture(const std::string& name, 
                      const std::string& path,
                      int size);

    /**
    * Creates a texture of random normals used for ambient occlusion
    */
    void MakeRandomNormals();

};