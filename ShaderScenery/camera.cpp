#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <sstream>
#include "camera.h"
#include "diagnostic.h"

Camera::CameraTypes Camera::sm_activeType;

Camera::Camera():
    m_cameraTarget(nullptr),
    m_cameraFree(nullptr),
    m_cameraKey(nullptr)
{
}

Camera::~Camera()
{
}

bool Camera::Initialise()
{
    m_cameraTarget = Scene()->addCameraSceneNodeMaya();
    m_cameraFree = Scene()->addCameraSceneNodeMaya();
    m_cameraKey = Scene()->addCameraSceneNode();
    return LoadCameraFromFile();
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

    ISceneNode* node = Scene()->getSceneNodeFromName(focus.c_str());
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

    Scene()->setActiveCamera(m_cameraTarget);
    sm_activeType = TARGET;

    return true;
}

void Camera::ReloadCameraFromFile()
{
    m_cameraTarget = nullptr;
    Scene()->getActiveCamera()->remove();
    m_cameraTarget = Scene()->addCameraSceneNodeMaya();

    LoadCameraFromFile();
    Diagnostic::Get()->ShowDiagnosticText(L"Camera Reloaded");
}

void Camera::ToggleCameraTarget(bool free)
{
    Scene()->setActiveCamera(free ? m_cameraFree : m_cameraTarget);
    sm_activeType = free ? FREE : TARGET;
}

void Camera::LoadKeyedCamera()
{
    sm_activeType = KEYED;
}

std::wstring Camera::GetCameraType()
{
    switch(sm_activeType)
    {
    case FREE:
        return L"FREE CAMERA";
    case TARGET:
        return L"TARGET CAMERA";
    case KEYED:
        return L"KEYED CAMERA";
    default:
        return L"NONE";
    }
}

void Camera::SetComponentValue(unsigned int component, float value)
{
    const float delta = 0.00001f;
    auto* camera = Scene()->getActiveCamera();
    switch(component)
    {
    case NEAR_VALUE:
        camera->setNearValue((camera->getFarValue() == value ? value+delta : value));
        break;
    case FAR_VALUE:
        camera->setFarValue((camera->getNearValue() == value ? value+delta : value));
        break;
    }
}

stringw Camera::GetComponentDescription(unsigned int component)
{
    switch(component)
    {
    case NEAR_VALUE:
        return "NEAR VALUE";
    case FAR_VALUE:
        return "FAR VALUE";
    default:
        return "NONE";
    }
}