////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "cache.h"
#include "qt/project/editor.h"
#include "qt/project/tweaker.h"
#include "qt/include/QtWidgets/qapplication.h"

Gui::Gui(std::shared_ptr<Cache> cache) :
    m_cache(cache)
{
}

Gui::~Gui()
{
}

void Gui::Run(int argc, char *argv[])
{
    Logger::LogInfo("Initialising Qt");
    QApplication app(argc, argv);

    SignalCallbacks callbacks;
    callbacks.SetLightPositionX =   [&](float x){ m_cache->LightPosition.SetX(x); };
    callbacks.SetLightPositionY =   [&](float y){ m_cache->LightPosition.SetY(y); };
    callbacks.SetLightPositionZ =   [&](float z){ m_cache->LightPosition.SetZ(z); };
    callbacks.SetLightAttX =        [&](float x){ m_cache->LightAttenuation.SetX(x); };
    callbacks.SetLightAttY =        [&](float y){ m_cache->LightAttenuation.SetY(y); };
    callbacks.SetLightAttZ =        [&](float z){ m_cache->LightAttenuation.SetZ(z); };
    callbacks.SetLightDiffuseR =    [&](float r){ m_cache->LightDiffuse.SetR(r); };
    callbacks.SetLightDiffuseG =    [&](float g){ m_cache->LightDiffuse.SetG(g); };
    callbacks.SetLightDiffuseB =    [&](float b){ m_cache->LightDiffuse.SetB(b); };
    callbacks.SetLightSpecularR =   [&](float r){ m_cache->LightSpecular.SetR(r); };
    callbacks.SetLightSpecularG =   [&](float g){ m_cache->LightSpecular.SetG(g); };
    callbacks.SetLightSpecularB =   [&](float b){ m_cache->LightSpecular.SetB(b); };    
    callbacks.SetLightSpecularity = [&](float s){ m_cache->LightSpecularity.Set(s); };  
    callbacks.SetMeshSpecularity =  [&](float s){ m_cache->MeshSpecularity.Set(s); };  
    callbacks.SetSelectedEngine =   [&](int index){ m_cache->EngineSelected.Set(index); };
    callbacks.SetSelectedMesh =     [&](int index){ m_cache->MeshSelected.Set(index); };
    callbacks.SetSelectedLight =    [&](int index){ m_cache->LightSelected.Set(index); };
    callbacks.SetSelectedShader =   [&](int index){ m_cache->ShaderSelected.Set(index); };
    callbacks.CompileShader =       [&](){ m_cache->CompileShader.Set(true); };

    Editor editor(callbacks);
    editor.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    editor.show();

    Tweaker tweaker(callbacks);
    tweaker.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    tweaker.show();

    while(m_cache->ApplicationRunning.Get())
    {
        app.processEvents();
        Update(tweaker, editor);
    }

    app.exit();
    Logger::LogInfo("Exiting Qt");
}

void Gui::Update(Tweaker& tweaker, Editor& editor)
{
    const auto page = ConvertStringToPage(tweaker.GetSelectedPage());
    if(page != m_page)
    {
        m_page = page;
        m_cache->PageSelected.Set(page);
    }

    switch(page)
    {
    case SCENE:
        UpdateScene(tweaker);
        break;
    case LIGHT:
        UpdateLight(tweaker);
        break;
    case MESH:
        UpdateMesh(tweaker);
        break;
    }

    bool initialisedShaders = false;
    if(!editor.HasShaders())
    {
        initialisedShaders = true;
        editor.InitialiseShaders(
            m_cache->ShaderSelected.Get(), m_cache->Shaders.Get());
    }

}

GuiPage Gui::ConvertStringToPage(const std::string& page)
{
    if(page == "Scene")
    {
        return SCENE;
    }
    else if(page == "Mesh")
    {
        return MESH;
    }
    else if(page == "Post")
    {
        return POST;
    }
    else if(page == "Light")
    {
        return LIGHT;
    }
    return NO_PAGE;
}

void Gui::UpdateScene(Tweaker& tweaker)
{
    if(!tweaker.HasEngines())
    {
        tweaker.InitialiseEngines(
            m_cache->EngineSelected.Get(), m_cache->Engines.Get());
    }

    const Float2 mousePosition = m_cache->MousePosition.Get();
    const Float2 mouseDirection = m_cache->MouseDirection.Get();
    const Float3 cameraPosition = m_cache->CameraPosition.Get();
    const float deltaTime = m_cache->DeltaTime.Get();
    const int framesPerSec = m_cache->FramesPerSec.Get();

    tweaker.SetMousePosition(
        boost::lexical_cast<std::string>(static_cast<int>(mousePosition.x)),
        boost::lexical_cast<std::string>(static_cast<int>(mousePosition.y)));

    tweaker.SetMouseDirection(
        boost::lexical_cast<std::string>(mouseDirection.x),
        boost::lexical_cast<std::string>(mouseDirection.y));

    tweaker.SetCameraPosition(
        boost::lexical_cast<std::string>(cameraPosition.x),
        boost::lexical_cast<std::string>(cameraPosition.y),
        boost::lexical_cast<std::string>(cameraPosition.z));

    tweaker.SetDeltaTime(boost::lexical_cast<std::string>(deltaTime));
    tweaker.SetFramesPerSec(boost::lexical_cast<std::string>(framesPerSec));
}

void Gui::UpdateLight(Tweaker& tweaker)
{
    const bool lightSwitched = m_cache->LightSwitched.Get();
    if(lightSwitched)
    {
        m_cache->LightSwitched.Set(false);
    }

    bool initialisedLights = false;
    if(!tweaker.HasLights())
    {
        initialisedLights = true;
        tweaker.InitialiseLights(
            m_cache->LightSelected.Get(), m_cache->Lights.Get());
    }

    // Only set once during initialisation or when a light is selected
    if(lightSwitched || initialisedLights)
    {
        const Float3 position = m_cache->LightPosition.Get();
        const Float3 attenuation = m_cache->LightAttenuation.Get();
        const Colour specular = m_cache->LightSpecular.Get();
        const Colour diffuse = m_cache->LightDiffuse.Get();

        tweaker.SetLightPosition(position.x, position.y, position.z);
        tweaker.SetLightAttenuation(attenuation.x, attenuation.y, attenuation.z);
        tweaker.SetLightDiffuse(diffuse.r, diffuse.g, diffuse.b);
        tweaker.SetLightSpecular(specular.r, specular.g, specular.b);
        tweaker.SetLightSpecularity(m_cache->LightSpecularity.Get());
    }
}

void Gui::UpdateMesh(Tweaker& tweaker)
{
    const bool meshSwitched = m_cache->MeshSwitched.Get();
    if(meshSwitched)
    {
        m_cache->MeshSwitched.Set(false);
    }

    bool initialisedMeshes = false;
    if(!tweaker.HasMeshes())
    {
        initialisedMeshes = true;
        tweaker.InitialiseMeshes(
            m_cache->MeshSelected.Get(), m_cache->Meshes.Get());
    }

    // Only set once during initialisation or when a mesh is selected
    if(meshSwitched || initialisedMeshes)
    {
        tweaker.SetMeshSpecularity(m_cache->MeshSpecularity.Get());
        tweaker.SetMeshBackFaceCull(m_cache->MeshBackFaceCull.Get());
        tweaker.SetMeshTransparency(m_cache->MeshTransparency.Get());
        tweaker.SetMeshDiffuseTexture(m_cache->MeshDiffuse.Get());
        tweaker.SetMeshSpecularTexture(m_cache->MeshSpecular.Get());
        tweaker.SetMeshNormalTexture(m_cache->MeshNormal.Get());
        tweaker.SetMeshShaderName(m_cache->MeshShader.Get());
    }
}
