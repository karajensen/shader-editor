////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene_builder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene_builder.h"
#include "scene_data.h"
#include "fragmentlinker.h"
#include "logger.h"

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

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
    * requires size of 3060 with fog 
    */
    const int PATCH_GRID_VERTICES = 35;
    const int PATCH_ROCK_TYPES = 3;
    const int PATCH_ROCK_INSTANCES = 20;
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
        return -1;
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
           InitialiseShadows() &&
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
    sun->Specular(Colour(0.0f, 100.0f/255.0f, 1.0f, 1.0f));
    sun->Specularity(15.0f);
    return true;
}

bool SceneBuilder::InitialiseShaders()
{
    bool success = true;
    m_data.shaders.resize(MAX_SHADERS);

    FragmentLinker linker;
    linker.Initialise(Water::Wave::MAX, m_data.lights.size(), m_data.post->GetBlurWeights());

    success &= InitialiseShader(linker, "post_effects", Shader::NONE, POST_SHADER);
    success &= InitialiseShader(linker, "pre_effects", Shader::NONE, PRE_SHADER);
    success &= InitialiseShader(linker, "blur_vertical", Shader::NONE, BLUR_VERTICAL_SHADER);
    success &= InitialiseShader(linker, "blur_horizontal", Shader::NONE, BLUR_HORIZONTAL_SHADER);
    success &= InitialiseShader(linker, "water", Shader::NONE, WATER_SHADER);
    success &= InitialiseShader(linker, "particle", Shader::NONE, PARTICLE_SHADER);
    success &= InitialiseShader(linker, "diagnostic", Shader::NONE, DIAGNOSTIC_SHADER);
    success &= InitialiseShader(linker, "shadow", Shader::NONE, SHADOW_SHADER);
    success &= InitialiseShader(linker, "bump", Shader::BUMP);
    success &= InitialiseShader(linker, "specular", Shader::SPECULAR);
    success &= InitialiseShader(linker, "diffusecaustics", Shader::CAUSTICS);
    success &= InitialiseShader(linker, "flat", Shader::FLAT);
    success &= InitialiseShader(linker, "bumpcaustics", Shader::CAUSTICS|Shader::BUMP);
    success &= InitialiseShader(linker, "bumpspecular", Shader::SPECULAR|Shader::BUMP);
    return success;
}

bool SceneBuilder::InitialiseTextures()
{
    bool success = true;

    success &= InitialiseTexture("blank", "blank.png", Texture::FROM_FILE);
    success &= InitialiseTexture("water_cube", "water_environment", Texture::CUBE, Texture::ANISOTROPIC);
    success &= InitialiseTexture("water_colour", "water.png", Texture::FROM_FILE);
    success &= InitialiseTexture("water_normal", "water_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bubble1", "bubble1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bubble2", "bubble2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bubble3", "bubble3.png", Texture::FROM_FILE);
    success &= InitialiseTexture("skybox", "skybox.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral1", "coral1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral2", "coral2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral_specular", "coral_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral_normal", "coral_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerA_top1", "flowerA_top1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerA_top2", "flowerA_top2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerA_top_specular", "flowerA_top_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerA_top_normal", "flowerA_top_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerB_top1", "flowerB_top1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerB_top2", "flowerB_top2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerB_top_specular", "flowerB_top_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerB_top_normal", "flowerB_top_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerC_top1", "flowerC_top1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerC_top2", "flowerC_top2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerC_top_specular", "flowerC_top_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerC_top_normal", "flowerC_top_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot", "flower_bot.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_specular", "flower_bot_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_normal", "flower_bot_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_large", "flower_bot_large.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_large_specular", "flower_bot_large_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_large_normal", "flower_bot_large_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("leaf", "leaf.png", Texture::FROM_FILE);
    success &= InitialiseTexture("leaf_specular", "leaf_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("shell1", "shell1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("shell2", "shell2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("starfish1", "starfish1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("starfish2", "starfish2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("starfish_normal", "starfish_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin1", "urchin1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin2", "urchin2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin_normal", "urchin_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin_specular", "urchin_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock", "rock.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock_bump", "rock_bump.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sand", "sand.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sand_bump", "sand_bump.png", Texture::FROM_FILE);
    success &= InitialiseTexture("shadow", "shadow.png", Texture::FROM_FILE);

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

    for (int i = 0; i < PATCH_ROCK_TYPES; ++i)
    {
        for (int j = 0; j < PATCH_ROCK_INSTANCES; ++j)
        {
            const auto index = m_data.rocks.size();
            m_data.rocks.emplace_back();
            m_data.rocks[index].index = m_data.terrain.size();
            m_data.rocks[index].instance = j;
        }

        const auto ID(std::to_string(i));
        Terrain& rock = InitialiseTerrain("rock" + ID, "terrain" + ID, 
            "bumpcaustics", 4.0f, false, 0.0f, -5.0f, 40.0f, 5.0f, 30);

        rock.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "rock"));
        rock.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "rock_bump"));
        rock.SetTexture(SLOT_CAUSTICS, causticsTexture);
        rock.AddInstances(PATCH_ROCK_INSTANCES);
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
    InitialiseMesh("diagnostic", "sphere.obj", 1.0f, 1.0f, "diagnostic");
    
    const int instances = 100;

    {
        auto& mesh = InitialiseMesh("skybox", "skybox.obj", 1.0f, 1.0f, "flat");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "skybox"));
        mesh.BackfaceCull(false);
        mesh.Ambience(0.85f);
        mesh.SetSkyBox();
        mesh.AddInstances(1);
    }
    {
        auto& mesh = InitialiseMesh("seaweed1", "seaweed1.obj", 0.25f, 4.0f, "specular");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "leaf"));
        mesh.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "leaf_specular"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, false, instances);
    }
    {
        auto& mesh = InitialiseMesh("seaweed2", "seaweed2.obj", 0.2f, 8.0f, "specular");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "leaf"));
        mesh.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "leaf_specular"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.Specular(0.3f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, false, instances);
    }
    {
        auto& mesh = InitialiseMesh("seaweed3", "seaweed3.obj", 0.2f, 8.0f, "specular");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "leaf"));
        mesh.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "leaf_specular"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.Specular(0.3f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, false, instances);
    }
    {
        auto& mesh = InitialiseMesh("shell", "shell.obj", 2.0f, 4.0f, "diffusecaustics");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "shell1"));
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "shell2"));
        mesh.SetTexture(SLOT_CAUSTICS, causticsTexture);
        mesh.CausticsScale(0.1f);
        mesh.CausticsAmount(0.2f);
        mesh.Specular(0.3f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, true, instances);
    }
    {
        auto& mesh = InitialiseMesh("starfish", "starfish.obj", 0.5f, 0.5f, "bump");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "starfish1"));
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "starfish2"));
        mesh.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "starfish_normal"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, true, instances);
    }
    {
        auto& mesh = InitialiseMesh("urchin", "urchin.obj", 1.0f, 1.0f, "bumpspecular");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "urchin1"));
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "urchin2"));
        mesh.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "urchin_normal"));
        mesh.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "urchin_specular"));
        mesh.Specular(0.5f);
        mesh.Bump(8.0f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, true, instances);
    }
    {
        auto& mesh = InitialiseMesh("coral", "coral.obj", 1.0f, 4.0f, "bumpspecular");
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "coral1"));
        mesh.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "coral2"));
        mesh.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "coral_normal"));
        mesh.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "coral_specular"));
        mesh.Bump(2.0f);
        mesh.Specular(0.5f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, true, instances);
    }
    {
        auto& mesh1 = InitialiseMesh("flower1_top", "flower1_top.obj", 1.0f, 1.0f, "bumpspecular");
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flowerA_top1"));
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flowerA_top2"));
        mesh1.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "flowerA_top_normal"));
        mesh1.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "flowerA_top_specular"));
        mesh1.Bump(2.5f);
        mesh1.Specular(0.5f);
        mesh1.AddInstances(instances);

        auto& mesh2 = InitialiseMesh("flower1_bot", "flower1_bot.obj", 2.0f, 2.0f, "bumpspecular");
        mesh2.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower_bot"));
        mesh2.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "flower_bot_normal"));
        mesh2.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "flower_bot_specular"));
        mesh2.AddInstances(instances);
        mesh2.Bump(2.5f);
        mesh2.Specular(0.4f);
        success &= AddFoliage({ &mesh1, &mesh2 }, true, instances);
    }
    {
        auto& mesh1 = InitialiseMesh("flower2_top", "flower2_top.obj", 1.0f, 1.0f, "bumpspecular");
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flowerB_top1"));
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flowerB_top2"));
        mesh1.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "flowerB_top_normal"));
        mesh1.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "flowerB_top_specular"));
        mesh1.Bump(2.0f);
        mesh1.Specularity(5.0f);
        mesh1.Specular(0.5f);
        mesh1.AddInstances(instances);
    
        auto& mesh2 = InitialiseMesh("flower2_bot", "flower2_bot.obj", 3.0f, 3.0f, "bumpspecular");
        mesh2.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower_bot_large"));
        mesh2.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "flower_bot_large_normal"));
        mesh2.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "flower_bot_large_specular"));
        mesh2.AddInstances(instances);
        mesh2.Specular(0.5f);
        mesh2.Bump(3.5f);
        success &= AddFoliage({ &mesh1, &mesh2 }, true, instances);
    }
    {
        auto& mesh1 = InitialiseMesh("flower3_top", "flower3_top.obj", 1.0f, 1.0f, "bumpspecular");
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flowerC_top1"));
        mesh1.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flowerC_top2"));
        mesh1.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "flowerC_top_normal"));
        mesh1.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "flowerC_top_specular"));
        mesh1.Bump(2.5f);
        mesh1.Specular(0.5f);
        mesh1.AddInstances(instances);

        auto& mesh2 = InitialiseMesh("flower3_bot", "flower3_bot.obj", 1.0f, 2.0f, "bumpspecular");
        mesh2.SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "flower_bot"));
        mesh2.SetTexture(SLOT_NORMAL, GetID(m_data.textures, "flower_bot_normal"));
        mesh2.SetTexture(SLOT_SPECULAR, GetID(m_data.textures, "flower_bot_specular"));
        mesh2.AddInstances(instances);
        mesh2.Bump(2.5f);
        mesh2.Specular(0.4f);
        success &= AddFoliage({ &mesh1, &mesh2 }, true, instances);
    }

    return success;
}

bool SceneBuilder::InitialiseShadows()
{
    m_data.shadows = std::make_unique<MeshData>("shadow", "shadow", SHADOW_SHADER);
    m_data.shadows->SetTexture(SLOT_DIFFUSE, GetID(m_data.textures, "shadow"));
    m_data.shadows->InitialiseMeshData();
    return true;
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
    data.instances = 120;
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
    if (index == -1)
    {
        index = m_data.shaders.size();
        m_data.shaders.push_back(std::make_unique<Shader>(name, components, true));
    }
    else
    {
        m_data.shaders[index] = std::make_unique<Shader>(name, components, false);
    }

    if (!linker.GenerateShader(*m_data.shaders[index]))
    {
        Logger::LogError("Could not generate shader " + name);
        return false;
    }
    else
    {
        Logger::LogInfo("Shader: " + name + " loaded");
        return true;
    }
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
        !m_data.shaders[shaderID]->HasComponent(Shader::FLAT),
        m_data.shaders[shaderID]->HasComponent(Shader::BUMP)))
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
    if (shaderID == -1)
    {
        Logger::LogError("Terrain: " + name + " could not find shader " + shaderName);
    }

    const int heightID = GetID(m_data.textures, heightmap);
    if (heightID == -1)
    {
        Logger::LogError("Terrain: " + name + " could not find texture " + heightmap);
    }

    const auto& texture = *m_data.textures[heightID];

    m_data.terrain.push_back(std::make_unique<Terrain>(
        name, m_data.shaders[shaderID]->Name(), shaderID, texture.Pixels()));
    Terrain& terrain = *m_data.terrain[m_data.terrain.size()-1];

    if (!terrain.Initialise(uvTextureStretch, minHeight, 
        maxHeight, height, spacing, size,
        !m_data.shaders[shaderID]->HasComponent(Shader::FLAT),
        m_data.shaders[shaderID]->HasComponent(Shader::BUMP), tiling))
    {
        Logger::LogError("Terrain: " + name + " failed initialisation");
    }
    return terrain;
}

bool SceneBuilder::AddFoliage(std::initializer_list<const MeshData*> meshes, bool hasShadow, int instances)
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

        if (hasShadow)
        {
            const int shadowID = static_cast<int>(m_data.shadows->Instances().size());
            m_data.shadows->AddInstances(1);
            m_data.foliage[index].AddShadow(shadowID);
        }

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
