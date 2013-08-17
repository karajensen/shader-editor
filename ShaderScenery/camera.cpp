////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "camera.h"
#include "diagnostic.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <sstream>

Camera::Camera(EnginePtr engine):
    m_engine(engine),
    m_cameraTarget(nullptr),
    m_cameraFree(nullptr),
    m_cameraKey(nullptr),
    m_nearValue(0.0f),
    m_farValue(0.0f)
{
    m_cameraTarget = m_engine->scene->addCameraSceneNodeMaya();
    m_cameraFree = m_engine->scene->addCameraSceneNodeMaya();
    m_cameraKey = m_engine->scene->addCameraSceneNode();
    LoadCameraFromFile();
}

Camera::~Camera()
{
}

bool Camera::LoadCameraFromFile()
{
    boost::property_tree::ptree meshes;
    boost::property_tree::xml_parser::read_xml(ASSETS_PATH+"Camera.xml", 
        meshes, boost::property_tree::xml_parser::trim_whitespace);
    boost::property_tree::ptree& tree = meshes.get_child("Camera");

    std::string focus = tree.get_child("Focus").data();
    float zoom = boost::lexical_cast<float>(tree.get_child("Zoom").data());
    float near = boost::lexical_cast<float>(tree.get_child("Near").data());
    float far = boost::lexical_cast<float>(tree.get_child("Far").data());

    ISceneNode* node = m_engine->scene->getSceneNodeFromName(focus.c_str());
    if(!node)
    {
        Logger::LogError("Could not find mesh " + focus + " to focus camera on");
        return false;
    }

    auto setCameraValues = [=](ICameraSceneNode* camera){ 
        camera->setNearValue(near); camera->setFarValue(far); };

    setCameraValues(m_cameraTarget);
    setCameraValues(m_cameraFree);
    setCameraValues(m_cameraKey);
   
    vector3df target(node->getBoundingBox().getCenter());
    vector3df targetPos(target);
    targetPos.normalize();
    targetPos *= -zoom;

    m_cameraTarget->setTarget(target);
    m_cameraTarget->setPosition(target+targetPos);
    m_cameraTarget->setRotation(vector3df(0,0,0));
    m_cameraTarget->updateAbsolutePosition();

    ChangeCameraType(TARGET);
    return true;
}

void Camera::ReloadCameraFromFile()
{
    m_cameraTarget = nullptr;
    m_engine->scene->getActiveCamera()->remove();
    m_cameraTarget = m_engine->scene->addCameraSceneNodeMaya();

    LoadCameraFromFile();
}

void Camera::ToggleCameraTarget(bool free)
{
    ChangeCameraType(free ? FREE : TARGET);
}

void Camera::ChangeCameraType(Camera::CameraType type)
{
    m_activeType = type;
    switch(m_activeType)
    {
    case FREE:
        m_engine->scene->setActiveCamera(m_cameraFree);
        m_typeName = "Free Camera";
        break;
    case TARGET:
        m_engine->scene->setActiveCamera(m_cameraTarget);
        m_typeName = "Target Camera";
        break;
    case KEYED:
        m_typeName = "Keyed Camera";
        break;
    default:
        m_typeName = "None";
    }

    m_farValue = m_engine->scene->getActiveCamera()->getFarValue();
    m_nearValue = m_engine->scene->getActiveCamera()->getNearValue();
}

void Camera::LoadKeyedCamera()
{
    ChangeCameraType(KEYED);
}

void Camera::Update()
{
    auto* camera = m_engine->scene->getActiveCamera();

    if(m_farValue == m_nearValue)
    {
        // ensure near and far are never equal
        m_farValue += 0.00001f;
    }

    camera->setNearValue(m_nearValue);
    camera->setFarValue(m_farValue);
}

const char* Camera::GetComponentDescription(Camera::EditableComponent component) const
{
    switch(component)
    {
    case NEAR_VALUE:
        return "Near Value";
    case FAR_VALUE:
        return "Far Value";
    default:
        return "None";
    }
}

const char* Camera::GetCameraType()
{
    return m_typeName.c_str();
}

void* Camera::GetComponentAddress(Camera::EditableComponent component)
{
    switch(component)
    {
    case NEAR_VALUE:
        return &m_nearValue;
    case FAR_VALUE:
        return &m_farValue;
    default:
        return nullptr;
    }
}