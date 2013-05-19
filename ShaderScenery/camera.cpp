#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include "camera.h"
#include "game.h"

Camera::Camera():
    m_cameraTarget(nullptr),
    m_cameraFree(nullptr),
    m_cameraKey(nullptr)
{
}

Camera::~Camera()
{
}

bool Camera::Initialise(const std::string& assetsPath)
{
    m_cameraTarget = Scene()->addCameraSceneNodeMaya();
    m_cameraFree = Scene()->addCameraSceneNodeMaya();
    m_cameraKey = Scene()->addCameraSceneNode();
    return LoadCameraFromFile(assetsPath);
}

bool Camera::LoadCameraFromFile(const std::string& assetsPath)
{
    boost::property_tree::ptree meshes;
    boost::property_tree::xml_parser::read_xml(assetsPath+"Camera.xml", 
        meshes, boost::property_tree::xml_parser::trim_whitespace);
    boost::property_tree::ptree& tree = meshes.get_child("Camera");

    std::string focus = tree.get_child("Focus").data();
    float zoom = boost::lexical_cast<float>(tree.get_child("Zoom").data());

    ISceneNode* node = Scene()->getSceneNodeFromName(focus.c_str());
    if(!node)
    {
        Logger::LogError("Could not find mesh " + focus + " to focus camera on");
        return false;
    }
   
    vector3df target(node->getBoundingBox().getCenter());
    vector3df targetPos(target);
    targetPos.normalize();
    targetPos *= -zoom;

    m_cameraTarget->setTarget(target);
    m_cameraTarget->setPosition(target+targetPos);
    m_cameraTarget->setRotation(vector3df(0,0,0));
    m_cameraTarget->updateAbsolutePosition();
    Scene()->setActiveCamera(m_cameraTarget);
    return true;
}

void Camera::ReloadCameraFromFile(const std::string& assetsPath)
{
    m_cameraTarget = nullptr;
    Scene()->getActiveCamera()->remove();
    m_cameraTarget = Scene()->addCameraSceneNodeMaya();

    LoadCameraFromFile(assetsPath);
    ShowDiagnosticText(L"Camera Reloaded");
}

void Camera::ToggleCameraTarget(bool free)
{
    Scene()->setActiveCamera(free ? m_cameraFree : m_cameraTarget);
}

void Camera::LoadKeyedCamera(const std::string& assetsPath)
{




}