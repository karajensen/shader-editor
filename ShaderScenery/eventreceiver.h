////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - eventreciver.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include <functional>
#include <unordered_map>

/**
* Derived class for overriding events in Irrlicht
*/
class EventReceiver : public IEventReceiver
{
public:

    typedef std::function<void(void)> KeyFn;

    /**
    * Irrlicht override
    * @param event The irrlicht event
    * @return true if handled, false if irrlicht needs to also handle it
    */
    virtual bool OnEvent(const SEvent& event);

    /**
    * Set the function to call upon toggle of the input key
    * @param key The DirectInput keyboard scan code
    * @param onContinuous Whether to call on key press or key press continously
    * @param keyFn The function to call when key is pressed
    */
    void SetKeyCallback(unsigned int key, bool onContinuous, KeyFn keyFn);

    /**
    * Checks what keys are pressed and calls the relevent callbacks
    */
    void Update();

private:

    /**
    * Masks used for the key array
    */
    enum
    {
        KEY_DOWN = 1,
        KEY_QUERIED = 2
    };

    /**
    * Key object structure
    */
    struct Key
    {
        /**
        * Constructor
        */
        Key();

        unsigned int state;  ///< Current state of the key
        KeyFn onKeyFn;       ///< Function to call if key is down
        bool continuous;     ///< Whether key should look at continous or not
    };

    /**
    * Whether a key is currently being pressed or not
    * @param key The state of the key
    * @param whether the key is being pressed
    */
    bool IsKeyDownContinuous(unsigned int& state);

    /**
    * Whether a key was pressed. Once queried will return false until key is released
    * @param state The state of the key
    * @param whether the key was pressed
    */
    bool IsKeyDown(unsigned int& state);

    typedef std::unordered_map<unsigned int, Key> KeyMap;
    KeyMap m_keys; ///< Cached keys
};