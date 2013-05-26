/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* Camera manager class
*****************************************************************/
#pragma once

#include <boost/noncopyable.hpp>
#include "common.h"

class Camera : boost::noncopyable
{
public:

    Camera();
    ~Camera();

    /**
    * Camera components avaliable for editing
    */
    enum Component
    {
        NEAR_VALUE,
        FAR_VALUE
    };

    /**
    * Creates the main camera
    */
    bool Initialise();

    /**
    * Loads the camera from file
    */
    bool LoadCameraFromFile();

    /**
    * Loads the camera position/rotational 
    * information for a keyed path from file
    * @param the path to where the file exists
    */
    void LoadKeyedCamera();

    /**
    * Reloads camera from file
    * @param the path to where the file exists
    */
    void ReloadCameraFromFile();

    /**
    * Toggles the camera between free roam and targeted
    * @param whether free roam (or targeted if false)
    */
    void ToggleCameraTarget(bool free);

    /**
    * @return a string describing the camera
    */
    static std::wstring GetCameraType();

    /**
    * Sets the value of the editable component
    * @param the component
    * @param the value to set the component
    */
    static void SetComponentValue(unsigned int component, float value);

    /**
    * @return a string description of the editable component
    */
    static stringw GetComponentDescription(unsigned int component);

private:

    enum CameraTypes
    {
        FREE,
        TARGET,
        KEYED
    };

    static CameraTypes sm_activeType; ///< Currently active camera type
    ICameraSceneNode* m_cameraTarget; ///< Camera that targets a specific mesh
    ICameraSceneNode* m_cameraFree;   ///< Free roaming camera
    ICameraSceneNode* m_cameraKey;    ///< Camera using xml for pos/rot

};