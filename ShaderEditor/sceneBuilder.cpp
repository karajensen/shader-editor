////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneBuilder.h"
#include "sceneData.h"
#include "fragmentlinker.h"
#include "boost/algorithm/string.hpp"

namespace
{
    /**
    * Resource file information
    */
    const std::string MESHES_PATH(ASSETS_PATH + "Meshes//");
    const std::string TEXTURE_PATH(ASSETS_PATH + "Textures//");
    const std::string GENERATED_TEXTURES(TEXTURE_PATH + "Generated//");

    /**
    * Shared values for creating meshes
    */
    const int PATCH_GRID_VERTICES = 51;
    const int PATCH_ROCK_TYPES = 3;
    const float PATCH_GRID_SPACING = 10.0f;

    /**
    * Helper function to get an asset by name
    */
    template<typename T> 
    int GetID(const std::vector<T>& data, const std::string& name)
    {
        for (unsigned int i = 0; i < data.size(); ++i)
        {
            if (boost::iequals(name, data[i]->Name()))
            {
                return i;
            }
        }
        Logger::LogError("Could not find " + name);
        return NO_INDEX;
    }
}

SceneBuilder::SceneBuilder(SceneData& data) :
    m_data(data)
{
}

SceneBuilder::~SceneBuilder() = default;

bool SceneBuilder::Initialise()
{
    return InitialiseLighting() &&
           InitialiseTextures() &&
           InitialiseShaders() &&
           InitialiseEmitters() &&
           InitialiseWater() &&
           InitialiseMeshes() && 
           InitialiseTerrain() && 
           InitialiseDiagnostics();
}

bool SceneBuilder::InitialiseLighting()
{
    m_data.sunIndex = m_data.lights.size();
    m_data.lights.push_back(std::make_unique<Light>("Sun"));
    auto& sun = m_data.lights[m_data.sunIndex];
    sun->Position(Float3(0.0, 60.0, 0.0));
    sun->Attenuation(Float3(1.0, 0.0, 0.0));
    sun->Diffuse(Colour(0.85f, 0.85f, 1.0f, 1.0f));
    sun->Specular(Colour(1.0f, 1.0f, 1.0f, 1.0f));
    sun->Specularity(20.0f);
    return true;
}

bool SceneBuilder::InitialiseShaders()
{
    bool success = true;
    m_data.shaders.resize(MAX_SHADERS);

    FragmentLinker linker;
    linker.Initialise(m_data.lights.size(), *m_data.post);

    success &= InitialiseShader(linker, "post_effects", Shader::NONE, POST_SHADER);
    success &= InitialiseShader(linker, "pre_effects", Shader::NONE, PRE_SHADER);
    success &= InitialiseShader(linker, "blur_vertical", Shader::NONE, BLUR_VERTICAL_SHADER);
    success &= InitialiseShader(linker, "blur_horizontal", Shader::NONE, BLUR_HORIZONTAL_SHADER);
    success &= InitialiseShader(linker, "water", Shader::NONE, WATER_SHADER);
    success &= InitialiseShader(linker, "particle", Shader::NONE, PARTICLE_SHADER);
    success &= InitialiseShader(linker, "diagnostic", Shader::NONE, DIAGNOSTIC_SHADER);
    success &= InitialiseShader(linker, "diffuse", Shader::NONE);
    success &= InitialiseShader(linker, "bump", Shader::BUMP);
    success &= InitialiseShader(linker, "diffusecaustics", Shader::CAUSTICS);
    success &= InitialiseShader(linker, "flat", Shader::NONE);
    success &= InitialiseShader(linker, "bumpcaustics", Shader::CAUSTICS|Shader::BUMP);
    success &= InitialiseShader(linker, "bumpspecularcaustics", Shader::BUMP|Shader::SPECULAR|Shader::CAUSTICS);
    return success;
}

bool SceneBuilder::InitialiseTextures()
{
    bool success = true;

    success &= InitialiseTexture("blank", "blank.png", Texture::FROM_FILE); // requires first index
    success &= InitialiseTexture("water_cube", "water_environment", Texture::CUBE, Texture::ANISOTROPIC);
    success &= InitialiseTexture("water_colour", "water.png", Texture::FROM_FILE);
    success &= InitialiseTexture("water_normal", "water_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bubble1", "bubble1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bubble2", "bubble2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bubble3", "bubble3.png", Texture::FROM_FILE);
    success &= InitialiseTexture("skybox", "skybox.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral", "coral.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower2_top", "flower2_top.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower2_bot", "flower2_bot.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower3_top", "flower3_top.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower3_bot", "flower3_bot.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower1_top", "flower1_top.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower1_bot", "flower1_bot.png", Texture::FROM_FILE);
    success &= InitialiseTexture("leaf", "leaf.png", Texture::FROM_FILE);
    success &= InitialiseTexture("shell", "shell.png", Texture::FROM_FILE);
    success &= InitialiseTexture("starfish", "starfish.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin", "urchin.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock", "rock.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock_bump", "rock_bump.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock_specular", "rock_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sand", "sand.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sand_bump", "sand_bump.png", Texture::FROM_FILE);

    for (int i = 0; i < PATCH_ROCK_TYPES; ++i)
    {
        success &= InitialiseTexture("terrain" + 
            std::to_string(i), ProceduralTexture::PERLIN_NOISE_ROCK, 128);
    }

    success &= InitialiseTexture("sand_height", ProceduralTexture::FROM_FILE, 256);

    return success && InitialiseCaustics();
}

bool SceneBuilder::InitialiseTerrain()
{
    const int causticsTexture = m_data.caustics->GetFrame();

    m_data.sandIndex = m_data.terrain.size();
    Terrain& sand = InitialiseTerrain("sand", "sand_height", 
        "bumpcaustics", 4.0f, true, -45.0f, 
        0.0f, 5.0f, PATCH_GRID_SPACING, PATCH_GRID_VERTICES);

    sand.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "sand"));
    sand.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "sand_bump"));
    sand.SetTexture(SLOT_CAUSTICS, causticsTexture);
    sand.Bump(10.0f);
    sand.Ambience(1.0f);
    sand.CausticsAmount(0.2f);

    const int instancesPerType = 10;
    for (int i = 0; i < PATCH_ROCK_TYPES; ++i)
    {
        for (int j = 0; j < instancesPerType; ++j)
        {
            const auto index = m_data.rocks.size();
            m_data.rocks.emplace_back();
            m_data.rocks[index].index = m_data.terrain.size();
            m_data.rocks[index].instance = j;
        }

        const auto ID(std::to_string(i));
        Terrain& rock = InitialiseTerrain("rock" + ID, "terrain" + ID, 
            "bumpcaustics", 4.0f, false, 0.0f, -5.0f, 40.0f, 5.0f, 30);

        rock.AddInstances(instancesPerType);
        rock.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "rock"));
        rock.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "rock_bump"));
        rock.SetTexture(SLOT_CAUSTICS, causticsTexture);
        rock.Bump(15.0f);
        rock.CausticsAmount(0.8f);
    }

    return true;
}

bool SceneBuilder::InitialiseWater()
{
    const auto index = m_data.water.size();
    m_data.water.push_back(std::make_unique<Water>(
        "water", m_data.shaders[WATER_SHADER]->Name(), WATER_SHADER));
    auto& water = *m_data.water[index];

    water.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "water_colour"));
    water.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "water_normal"));
    water.SetTexture(SLOT_ENVIRONMENT, GetID(m_data.textures, "water_cube"));
    return water.Initialise(25.0f, PATCH_GRID_SPACING, PATCH_GRID_VERTICES);
}

bool SceneBuilder::InitialiseMeshes()
{
    bool success = true;

    const int causticsTexture = m_data.caustics->GetFrame();

    {
        auto& mesh = InitialiseMesh("sphere", "sphere.obj", 1.0f, 1.0f, "diagnostic");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "blank"));
    }
    {
        auto& mesh = InitialiseMesh("skybox", "skybox.obj", 1.0f, 1.0f, "flat");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "skybox"));
        mesh.BackfaceCull(false);
        mesh.Ambience(0.85f);
        mesh.SetSkyBox();
        mesh.AddInstances(1);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("sealeaves", "sealeaves.obj", 0.25f, 4.0f, "diffuse");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "leaf"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("seaweed1", "seaweed1.obj", 0.25f, 4.0f, "diffuse");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "leaf"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("seaweed2", "seaweed2.obj", 0.2f, 8.0f, "diffuse");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "leaf"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("shell", "shell.obj", 2.0f, 4.0f, "diffuse");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "shell"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("starfish", "starfish.obj", 0.5f, 0.5f, "diffuse");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "starfish"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("urchin", "urchin.obj", 1.0f, 1.0f, "diffuse");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "urchin"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("coral", "coral.obj", 1.0f, 4.0f, "diffuse");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "coral"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh1 = InitialiseMesh("flower1 top", "flower1_top.obj", 1.0f, 1.0f, "diffuse");
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower1_top"));
        mesh1.AddInstances(instances);

        auto& mesh2 = InitialiseMesh("flower1 bot", "flower1_bot.obj", 2.0f, 2.0f, "diffuse");
        mesh2.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower1_bot"));
        mesh2.AddInstances(instances);
        success &= AddFoliage({ &mesh1, &mesh2 }, instances);
    }
    {
        const int instances = 80;
        auto& mesh1 = InitialiseMesh("flower2 top", "flower2_top.obj", 1.0f, 1.0f, "diffuse");
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower2_top"));
        mesh1.AddInstances(instances);
    
        auto& mesh2 = InitialiseMesh("flower2 bot", "flower2_bot.obj", 3.0f, 3.0f, "diffuse");
        mesh2.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower2_bot"));
        mesh2.AddInstances(instances);
        success &= AddFoliage({ &mesh1, &mesh2 }, instances);
    }
    {
        const int instances = 80;
        auto& mesh1 = InitialiseMesh("flower3 top", "flower3_top.obj", 1.0f, 1.0f, "diffuse");
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower3_top"));
        mesh1.AddInstances(instances);

        auto& mesh2 = InitialiseMesh("flower3 bot", "flower3_bot.obj", 1.0f, 2.0f, "diffuse");
        mesh2.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower3_bot"));
        mesh2.AddInstances(instances);
        success &= AddFoliage({ &mesh1, &mesh2 }, instances);
    }

    return success;
}

bool SceneBuilder::InitialiseEmitters()
{
    return InitialiseBubbles();
}

bool SceneBuilder::InitialiseBubbles()
{
    std::vector<int> textures = 
    {
        GetID(m_data.textures, "bubble1"),
        GetID(m_data.textures, "bubble2"),
        GetID(m_data.textures, "bubble3")
    };

    EmitterData data;
    data.direction.x = 0.0f;
    data.direction.y = 1.0f;
    data.direction.z = 0.0f;
    data.length = 10.0f;
    data.width = 10.0f;
    data.lifeTime = 5.0f;
    data.lifeFade = 0.5f;
    data.maxAmplitude = 1.5f;
    data.minAmplitude = 0.5f;
    data.maxFrequency = 1.0f;
    data.minFrequency = 0.5f;
    data.maxWaveSpeed = 1.0f;
    data.minWaveSpeed = 1.0f;
    data.maxSpeed = 0.3f;
    data.minSpeed = 0.2f;
    data.minSize = 0.5f;
    data.maxSize = 1.5f;
    data.minWaitTime = 0.5f;
    data.maxWaitTime = 3.0f;    
    data.tint.r = 1.0f;
    data.tint.g = 1.0f;
    data.tint.b = 1.0f;
    data.tint.a = 1.0f;
    data.instances = 100;
    data.particles = 15;

    return InitialiseEmitter("bubbles", "particle", textures, data);
}

bool SceneBuilder::InitialiseCaustics()
{
    m_data.caustics = std::make_unique<Animation>();

    const std::string path("Caustics//");
    const std::string name("Caustics_0");
    const std::string extension(".bmp");

    const int maxFrames = 45;
    for (int frame = 1; frame <= maxFrames; ++frame)
    {
        const std::string number(std::to_string(frame));
        const std::string frameName(name + (frame < 10 ? "0" : "") + number + extension);
        m_data.caustics->AddFrame(static_cast<int>(m_data.textures.size()));

        if (!InitialiseTexture(frameName, path + frameName, Texture::FROM_FILE))
        {
            return false;
        }
    }

    Logger::LogInfo("Caustics: Initialised " + std::to_string(maxFrames) + " frames");
    return true;
}

bool SceneBuilder::InitialiseShader(FragmentLinker& linker,
                                    const std::string& name, 
                                    unsigned int components,
                                    int index)
{
    if (index == NO_INDEX)
    {
        index = m_data.shaders.size();
        m_data.shaders.push_back(std::make_unique<Shader>(name, SHADER_PATH + name, components));
        if (!linker.GenerateWithFragments(*m_data.shaders[index]))
        {
            Logger::LogError("Could not generate shader " + name);
        }
    }
    else
    {
        m_data.shaders[index] = std::make_unique<Shader>(name, SHADER_PATH + name, components);
        if (!linker.GenerateFromFile(*m_data.shaders[index]))
        {
            Logger::LogError("Could not generate shader " + name);
            return false;
        }
    }

    Logger::LogInfo("Shader: " + name + " loaded");
    return true;
}

bool SceneBuilder::InitialiseTexture(const std::string& name, 
                                     const std::string& path,
                                     Texture::Type type,
                                     Texture::Filter filter)
{
    const auto index = m_data.textures.size();
    m_data.textures.push_back(std::make_unique<Texture>(name, TEXTURE_PATH + path, type, filter));
    return true;
}

bool SceneBuilder::InitialiseTexture(const std::string& name, 
                                     ProceduralTexture::Generation generation,
                                     int size)
{
    const auto index = m_data.textures.size();
    m_data.proceduralTextures.push_back(index);

    const std::string path(generation == ProceduralTexture::FROM_FILE ? 
        TEXTURE_PATH + name + ".png" : GENERATED_TEXTURES + name + ".bmp");

    m_data.textures.push_back(std::make_unique<ProceduralTexture>(
        name, path, size, generation));

    m_data.textures[index]->Save();
    return true;
}

Mesh& SceneBuilder::InitialiseMesh(const std::string& name,
                                   const std::string& filename,
                                   float uScale,
                                   float vScale,
                                   const std::string& shaderName)
{
    const auto index = m_data.meshes.size();
    const int shaderID = GetID(m_data.shaders, shaderName);

    m_data.meshes.push_back(std::make_unique<Mesh>(name, shaderName, shaderID));
    auto& mesh = *m_data.meshes[index];

    if (!mesh.InitialiseFromFile(MESHES_PATH + filename, Float2(uScale, vScale),
        true, m_data.shaders[shaderID]->HasComponent(Shader::BUMP)))
    {
        Logger::LogError("Mesh: " + name + " failed initialisation");
    }

    return mesh;
}

bool SceneBuilder::InitialiseEmitter(const std::string& name,
                                     const std::string& shaderName,
                                     const std::vector<int>& textures,
                                     const EmitterData& data)
{
    const int shaderID = GetID(m_data.shaders, shaderName);
    m_data.emitters.push_back(std::make_unique<Emitter>(name, shaderID));
    Emitter& emitter = *m_data.emitters[m_data.emitters.size()-1];

    for (int texture : textures)
    {
        emitter.AddTexture(texture);
    }

    return emitter.Initialise(data);
}

Terrain& SceneBuilder::InitialiseTerrain(const std::string& name,
                                         const std::string& heightmap,
                                         const std::string& shaderName,
                                         float uvTextureStretch,
                                         bool tiling,
                                         float height,
                                         float minHeight,
                                         float maxHeight,
                                         float spacing,
                                         int size)
{
    const int shaderID = GetID(m_data.shaders, shaderName);
    if (shaderID == NO_INDEX)
    {
        Logger::LogError("Terrain: " + name + " could not find shader " + shaderName);
    }

    const int heightID = GetID(m_data.textures, heightmap);
    if (heightID == NO_INDEX)
    {
        Logger::LogError("Terrain: " + name + " could not find texture " + heightmap);
    }

    const auto& texture = *m_data.textures[heightID];

    m_data.terrain.push_back(std::make_unique<Terrain>(
        name, m_data.shaders[shaderID]->Name(), shaderID, texture.Pixels()));
    Terrain& terrain = *m_data.terrain[m_data.terrain.size()-1];

    if (!terrain.Initialise(uvTextureStretch, minHeight, 
        maxHeight, height, spacing, size, true, 
        m_data.shaders[shaderID]->HasComponent(Shader::BUMP), tiling))
    {
        Logger::LogError("Terrain: " + name + " failed initialisation");
    }
    return terrain;
}

bool SceneBuilder::AddFoliage(std::initializer_list<const MeshData*> meshes, int instances)
{
    if (meshes.size() == 0)
    {
        Logger::LogError("Empty foliage mesh group");
        return false;
    }

    for (int i = 0; i < instances; ++i)
    {
        const auto index = m_data.foliage.size();
        m_data.foliage.emplace_back();

        for (const auto* mesh : meshes)
        {
            if (static_cast<int>(mesh->Instances().size()) != instances)
            {
                Logger::LogError("Mesh " + mesh->Name() + " did not have require instances");
                return false;
            }

            m_data.foliage[index].AddMesh(GetID(m_data.meshes, mesh->Name()), i);
        }
    }
    return true;
}

bool SceneBuilder::InitialiseDiagnostics()
{
    bool created = false;
    for (const auto& mesh : m_data.meshes)
    {
        if (mesh->ShaderID() == DIAGNOSTIC_SHADER)
        {
            created = true;
            m_data.diagnostics = std::make_unique<Diagnostic>(mesh->Instances());
            break;
        }
    }

    if (!created)
    {
        Logger::LogError("Diagnostics: Could not find mesh");
        return false;
    }
    return true;
}
