////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshAttributes.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ptree_utilities.h"

/**
* Attributes used for shading meshes
*/
class MeshAttributes
{
public:

    /**
    * Constructor
    * @param node The data to intialize the mesh with
    */
    MeshAttributes(const boost::property_tree::ptree& node);

    /**
    * Writes the data to a property tree
    * @param node The node to write to
    */
    virtual void Write(boost::property_tree::ptree& node) const;

    /**
    * @return Brightness of the specular highlights
    */
    const float& Specularity() const;

    /**
    * Sets the Brightness of the specular highlights
    */
    void Specularity(float value);

    /**
    * @return The saturation of the bump
    */
    const float& Bump() const;

    /**
    * Sets The saturation of the bump
    */
    void Bump(float value);

    /**
    * @return Ambient light multiplier
    */
    const float& Ambience() const;

    /**
    * Sets the Ambient light multiplier
    */
    void Ambience(float value);

    /**
    * @return Diffuse light multiplier
    */
    const float& Diffuse() const;

    /**
    * Sets the Diffuse light multiplier
    */
    void Diffuse(float value);

    /**
    * @return Specular light multiplier
    */
    const float& Specular() const;

    /**
    * Sets the Specular light multiplier
    */
    void Specular(float value);

    /**
    * @return Caustics multiplier
    */
    const float& CausticsAmount() const;

    /**
    * Sets the Caustics multiplier
    */
    void CausticsAmount(float value);

    /**
    * @return Caustics scale
    */
    const float& CausticsScale() const;

    /**
    * Sets the Caustics scale
    */
    void CausticsScale(float value);

protected:

    float m_bump = 1.0f;           ///< Saturation of bump
    float m_causticsAmount = 1.0f; ///< How much of the caustics are visible
    float m_causticsScale = 1.0f;  ///< The scale of the caustic texture
    float m_specularity = 1.0f;    ///< Brightness of the specular highlights
    float m_ambience = 1.0f;       ///< Ambient light multiplier
    float m_specular = 1.0f;       ///< Specular light multiplier
    float m_diffuse = 1.0f;        ///< Diffuse light multiplier
};