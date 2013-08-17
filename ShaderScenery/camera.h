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
    enum EditableComponent
    {
        NEAR_VALUE,
        FAR_VALUE,
        MAX_COMPONENTS
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
    * Updates the camera editable components
    */
    void Update();

    /**
    * @return a string description of the editable component
    */
    const char* GetComponentDescription(EditableComponent component) const;

    /**
    * @param component The component to get the address of
    * @return the address of the component for diagnostics
    */
    void* GetComponentAddress(EditableComponent component);

    /**
    * @return a string description of the current camera
    */
    const char* GetCameraType();

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

    /**
    * Changes the currently active camera
    * @param type The camera to switch to
    */
    void ChangeCameraType(CameraType type);

    EnginePtr m_engine;               ///< Irrlicht engine
    CameraType m_activeType;          ///< Currently active camera type
    ICameraSceneNode* m_cameraTarget; ///< Camera that targets a specific mesh
    ICameraSceneNode* m_cameraFree;   ///< Free roaming camera
    ICameraSceneNode* m_cameraKey;    ///< Camera using xml for pos/rot
    float m_nearValue;                ///< Cached near value for diagnostics
    float m_farValue;                 ///< Cached far value for diagnostics
    std::string m_typeName;           ///< Cached name of selected camera for diagnostics
};