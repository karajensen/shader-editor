////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - grid.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "meshdata.h"

/**
* Holds information of a grid of vertices
*/
class Grid : public MeshData
{
public:

    /**
    * Constructor
    * @param node The data to intialize the mesh with
    */
    Grid(const boost::property_tree::ptree& node);

    /**
    * Writes the data to a property tree
    * @param node The node to write to
    */
    virtual void Write(boost::property_tree::ptree& node) const;

    /**
    * Loads a grid into the buffers
    * @param normals Whether to generate normals
    */
    void CreateGrid(bool normals);
                   
    /**
    * Resets the grid to default
    */
    void ResetGrid();

    /**
    * Sets the height at the given row/column
    */
    void Set(int row, int column, float height);

    /**
    * @return the value at the given row/column
    */
    float Get(int row, int column) const;

    /**
    * @return whether the given row/column is valid
    */
    bool Valid(int row, int column) const;

    /**
    * @return the number of rows of the grid
    */
    int Rows() const;

    /**
    * @return the number of columns of the grid
    */
    int Columns() const;

    /**
    * @return the initial position of the center of the grid
    */
    const Float3& Position() const;

    /**
    * Determines the normals for the grid
    */
    void RecalculateNormals();

private:

    float m_spacing = 0.0f;                     ///< The spacing between vertices
    int m_columns = 0;                          ///< The number of columns of this mesh
    int m_rows = 0;                             ///< The number of rows of this mesh
    bool m_hasNormals = false;                  ///< Whether to generate normals
    Float3 m_position;                          ///< The initial position of the grid
    std::vector<std::vector<float*>> m_heights; ///< Row x columns of the vertices
};