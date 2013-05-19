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
    * Creates the main camera
    * @param the path to where the file exists
    */
    bool Initialise(const std::string& assetsPath);

    /**
    * Loads the camera from file
    * @param the path to where the file exists
    */
    bool LoadCameraFromFile(const std::string& assetsPath);

    /**
    * Loads the camera position/rotational 
    * information for a keyed path from file
    * @param the path to where the file exists
    */
    void LoadKeyedCamera(const std::string& assetsPath);

    /**
    * Reloads camera from file
    * @param the path to where the file exists
    */
    void ReloadCameraFromFile(const std::string& assetsPath);

    /**
    * Toggles the camera between free roam and targeted
    * @param whether free roam (or targeted if false)
    */
    void ToggleCameraTarget(bool free);

private:

    ICameraSceneNode* m_cameraTarget; ///< Camera that targets a specific mesh
    ICameraSceneNode* m_cameraFree;   ///< Free roaming camera
    ICameraSceneNode* m_cameraKey;    ///< Camera using xml for pos/rot

};