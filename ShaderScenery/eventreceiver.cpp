////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - eventreciever.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "eventreceiver.h"

EventReceiver::Key::Key() :
    state(NONE),
    onKeyFn(nullptr),
    continuous(false)
{
}

bool EventReceiver::OnEvent(const SEvent& event)
{
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if(m_keys.find(static_cast<unsigned int>(event.KeyInput.Key)) != m_keys.end())
        {
            if(event.KeyInput.PressedDown)
            {
                // Turn on KEY_DOWN
                m_keys[event.KeyInput.Key].state |= KEY_DOWN;
            }
            else
            {
                // Turn off KEY_DOWN
                m_keys[event.KeyInput.Key].state &= ~KEY_DOWN;
            }
        }
    }
    else if(event.EventType == irr::EET_LOG_TEXT_EVENT)
    {
        Logger::LogInfo(event.LogEvent.Text);
        return true;
    }
    return false;
}

bool EventReceiver::IsKeyDownContinuous(unsigned int& state)
{
    return (state & KEY_DOWN) == KEY_DOWN;
}

bool EventReceiver::IsKeyDown(unsigned int& state)
{
    if(((state & KEY_DOWN) == KEY_DOWN) &&
       ((state & KEY_QUERIED) != KEY_QUERIED))
    {
        // Add user has queried for key to prevent constant queries
        state |= KEY_QUERIED;
        return true;
    }
	else if((state & KEY_DOWN) != KEY_DOWN)
    {
        // Key is lifted, allow user to query again
		state &= ~KEY_QUERIED;
    }
	return false;
}

void EventReceiver::Update()
{
    for(KeyMap::iterator it = m_keys.begin(); it != m_keys.end(); ++it)
    {
        if(it->second.continuous ? IsKeyDownContinuous(it->second.state) : IsKeyDown(it->second.state))
        {
            it->second.onKeyFn();
        }
    }
}

void EventReceiver::SetKeyCallback(unsigned int key, bool onContinuous, KeyFn keyFn)
{
    m_keys[key] = Key();
    m_keys[key].state = NONE;
    m_keys[key].continuous = onContinuous;
    m_keys[key].onKeyFn = keyFn;
}
