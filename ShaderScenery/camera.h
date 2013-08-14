////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include <boost/noncopyable.hpp>

/**
* Scene camera manager
*/
class Camera : boost::noncopyable
{
public:

    /**
    * Camera components avaliable for editing
    */
    enum Component
    {
        NEAR_VALUE,
        FAR_VALUE
    };

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit Camera(EnginePtr engine);

    /**
    * Destructor
    */
    ~Camera();

    /**
    * Loads the camera from file
    */
    bool LoadCameraFromFile();

    /**
    * Loads the camera position/rotational 
    * information for a keyed path from file
    */
    void LoadKeyedCamera();

    /**
    * Reloads camera from file
    */
    void ReloadCameraFromFile();

    /**
    * Toggles the camera between free roam and targeted
    * @param free Whether free roam (or targeted if false)
    */
    void ToggleCameraTarget(bool free);

    /**
    * @return a string describing the camera
    */
    std::wstring GetCameraType();

    /**
    * Sets the value of the editable component
    * @param component The component to set
    * @param value The value to set the component
    */
    void SetComponentValue(unsigned int component, float value);

    /**
    * @return a string description of the editable component
    */
    stringw GetComponentDescription(unsigned int component);

private:

    /**
    * Available cameras to switch into
    */
    enum CameraType
    {
        FREE,
        TARGET,
        KEYED
    };

    EnginePtr m_engine;               ///< Irrlicht engine
    CameraType m_activeType;          ///< Currently active camera type
    ICameraSceneNode* m_cameraTarget; ///< Camera that targets a specific mesh
    ICameraSceneNode* m_cameraFree;   ///< Free roaming camera
    ICameraSceneNode* m_cameraKey;    ///< Camera using xml for pos/rot
};