////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - elements.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <random>
#include "elements.h"
#include "common.h"
#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"

namespace
{
    static std::default_random_engine generator; ///< Used for random generation

    /**
    * Utility class to get a random value
    */
    int GetRandom(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

    /**
    * Utility class to get a random value
    */
    float GetRandom(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }
}

Float2::Float2(float X, float Y) 
    : x(X), y(Y)
{
}

PostProcessing::PostProcessing()
{
    SetPostMap(SCENE_MAP);

    weights[0] = 1.0f;
    weights[1] = 0.9f;
    weights[2] = 0.55f;
    weights[3] = 0.18f;
    weights[4] = 0.1f;
    NormaliseWeights();
}

void PostProcessing::NormaliseWeights()
{
    const float overallWeight = weights[0] + 2.0f * 
        (weights[1] + weights[2] + weights[3] + weights[4]);

    weights[0] /= overallWeight;
    weights[1] /= overallWeight;
    weights[2] /= overallWeight;
    weights[3] /= overallWeight;
    weights[4] /= overallWeight;
}

void PostProcessing::SetPostMap(PostProcessing::Map map)
{
    masks.assign(0.0f);
    masks[map] = 1.0f;
}

std::string PostProcessing::GetMapName(PostProcessing::Map map)
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

std::string Texture::GetTypeDescription(unsigned int type)
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

std::string Shader::ComponentAsString(unsigned int component)
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

bool Shader::HasComponent(unsigned int component) const
{
    return std::find(components.begin(), components.end(),
        Component(component)) != components.end();
}

Mesh::Mesh()
{
    textureIDs.resize(Texture::MAX_TYPES);
    textureIDs.assign(Texture::MAX_TYPES, NO_INDEX);
}

bool Mesh::Initialise(bool requiresNormals, bool requiresBumpMapping)
{
    const std::string path = ASSETS_PATH + "Meshes//" + name;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace|
        aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_SortByPType|
        aiProcess_CalcTangentSpace|aiProcess_JoinIdenticalVertices|aiProcess_GenSmoothNormals|
        aiProcess_LimitBoneWeights|aiProcess_RemoveRedundantMaterials|aiProcess_OptimizeMeshes);

    if(!scene)
    {
        Logger::LogError("Assimp import error for mesh " + path + ": " + importer.GetErrorString());
        return false;
    }

    if (requiresBumpMapping && !requiresNormals)
    {
        Logger::LogError(path + " requies normals for bump mapping");
        return false;
    }

    unsigned int numMeshes = scene->mNumMeshes;
    aiMesh** meshes = scene->mMeshes;

    // For each submesh
    bool generatedComponentCount = false;
    for(unsigned int i = 0; i < numMeshes; ++i)
    {
        aiMesh* pMesh = meshes[i];

        unsigned int indexOffset = vertices.size() / vertexComponentCount;
        vertexCount += pMesh->mNumVertices;
        faceCount += pMesh->mNumFaces;

        if(!pMesh->HasPositions())
        {
            Logger::LogError(name + " requires positions for requested shader");
            return false;
        }
        if(!pMesh->HasTextureCoords(0))
        {
            Logger::LogError(name + " requires uvs for requested shader");
            return false;
        }
        if(requiresNormals && !pMesh->HasNormals())
        {
            Logger::LogError(name + " requires normals for requested shader");
            return false;
        }

        // For each vertex
        int componentCount = 0;
        for(unsigned int vert = 0; vert < pMesh->mNumVertices; ++vert)
        {
            vertices.push_back(pMesh->mVertices[vert].x);
            vertices.push_back(pMesh->mVertices[vert].y);
            vertices.push_back(pMesh->mVertices[vert].z);
            vertices.push_back(pMesh->mTextureCoords[0][vert].x);
            vertices.push_back(pMesh->mTextureCoords[0][vert].y);
            componentCount = 5;

            if (requiresNormals)
            {
                vertices.push_back(pMesh->mNormals[vert].x);
                vertices.push_back(pMesh->mNormals[vert].y);
                vertices.push_back(pMesh->mNormals[vert].z);
                componentCount = 8;
            }

            // Add any bitangents/tangents for the mesh
            if(requiresBumpMapping)
            {
                if(pMesh->HasTangentsAndBitangents())
                {
                    vertices.push_back(pMesh->mTangents[vert].x);
                    vertices.push_back(pMesh->mTangents[vert].y);
                    vertices.push_back(pMesh->mTangents[vert].z);
                    vertices.push_back(pMesh->mBitangents[vert].x);
                    vertices.push_back(pMesh->mBitangents[vert].y);
                    vertices.push_back(pMesh->mBitangents[vert].z);
                    componentCount += 6;
                }
                else
                {
                    Logger::LogError(name + " requires tangents for requested shader");
                    return false;
                }
            }
        }

        // Make sure vertex layout is consistant between submeshes
        if(generatedComponentCount)
        {
            if(componentCount != vertexComponentCount)
            {
                Logger::LogError("Assimp error for mesh " + path + ": " + 
                    boost::lexical_cast<std::string>(componentCount) + " does not match " +
                    boost::lexical_cast<std::string>(vertexComponentCount));
                return false;
            }
        }
        else
        {
            vertexComponentCount = componentCount;
            generatedComponentCount = true;
        }

        // For each face
        for(unsigned int face = 0; face < pMesh->mNumFaces; ++face)
        {
            aiFace *pFace = &pMesh->mFaces[face];
            if(pFace->mNumIndices != 3)
            {
                Logger::LogError("Assimp error for mesh " + path + ": not all faces are triangles");
                return false;
            }

            indices.push_back(indexOffset + pFace->mIndices[0]);
            indices.push_back(indexOffset + pFace->mIndices[1]);
            indices.push_back(indexOffset + pFace->mIndices[2]);
        }
    }

    indexCount = indices.size();
    Logger::LogInfo("Mesh: " + path + " created");
    return true;
}

Particle::Particle() :
    maxWaitTime(GetRandom(0.0f,1.0f))
{
}

void Emitter::Resize(int size)
{
    const int currentSize = static_cast<int>(particles.size());
    if (size < currentSize)
    {
        particles.erase(particles.begin() + size, particles.end());
    }
    else if (size > currentSize)
    {
        particles.resize(size);
    }
}

void Emitter::TogglePaused()
{
    paused = !paused;
}

void Emitter::Tick(float deltatime)
{
    if (paused)
    {
        return;
    }

    for (Particle& particle : particles)
    {
        if (particle.alive)
        {
            particle.lifeTime += deltatime;
            particle.alive = particle.lifeTime < lifeTime;
            particle.position += direction * particle.speed;

            // Wave equation: y = a * sin(kx-wt+phase)
            const float wt = particle.waveSpeed * particle.lifeTime;
            const float kx = particle.frequency * particle.position.y;
            const float zOffset = 2.0f;
            particle.position.x = particle.amplitude * std::sin(kx - wt);
            particle.position.z = particle.amplitude * std::sin(kx - wt * zOffset);

            // Fade particle in/out of lifetime
            const float fadeEnd = lifeTime - lifeFade;
            if (particle.lifeTime <= lifeFade)
            {
                particle.alpha = Clamp(ConvertRange(particle.lifeTime, 
                    0.0f, lifeFade, 0.0f, 1.0f), 0.0f, 1.0f);
            }
            else if (particle.lifeTime >= fadeEnd)
            {
                particle.alpha = Clamp(ConvertRange(particle.lifeTime, 
                    fadeEnd, lifeTime, 1.0f, 0.0f), 0.0f, 1.0f);
            }
        }
        else if (particle.waitTime < particle.maxWaitTime)
        {
            particle.waitTime += deltatime;
        }
        else
        {
            particle.alpha = 0.0f;
            particle.waitTime = 0.0f;
            particle.lifeTime = 0.0f;
            particle.alive = true;
            particle.speed = GetRandom(minSpeed, maxSpeed);
            particle.size = GetRandom(minSize, maxSize);
            particle.texture = textures[GetRandom(0, static_cast<int>(textures.size()-1))];
            particle.amplitude = GetRandom(minAmplitude, maxAmplitude);
            particle.waveSpeed = GetRandom(minWaveSpeed, maxWaveSpeed);
            particle.frequency = GetRandom(minFrequency, maxFrequency);

            const float xOffset = GetRandom(-width, width) * 0.5f;
            const float zOffset = GetRandom(-length, length) * 0.5f;
            particle.position.x = position.x + xOffset;
            particle.position.z = position.z + zOffset;
            particle.position.y = position.y;
        }
    }

    std::stable_sort(particles.begin(), particles.end(), 
        [](const Particle& p1, const Particle& p2)
        {
            return p1.position.z < p2.position.z;
        });
}