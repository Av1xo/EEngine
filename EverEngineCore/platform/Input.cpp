#include "EverEngineCore/platform/Input.h"

void Input::init(EventDispatcher& dispatcher)
{
    dispatcher.add_event_listener<EventKeyPressed>(
        [this](EventKeyPressed& e) { handleKeyPressed(e); }
    );
    
    dispatcher.add_event_listener<EventKeyReleased>(
        [this](EventKeyReleased& e) { handleKeyReleased(e); }
    );
}

void Input::handleKeyPressed(EventKeyPressed& e)
{
    size_t index = toIndex(e.key);
    if (index >= KEY_COUNT) return;

    KeyState& state = m_keyStates[index];
    
    if (state.down && e.repeat)
    {
        state.repeated = true;
    }
    else if (!state.down)
    {
        state.down = true;
        state.pressedThisFrame = true;
        state.repeated = false;
    }

    if (!e.repeat)
    {
        for (auto& callback : m_pressCallbacks[index])
        {
            callback();
        }
    }
}

void Input::handleKeyReleased(EventKeyReleased& e)
{
    size_t index = toIndex(e.key);
    if (index >= KEY_COUNT) return;

    KeyState& state = m_keyStates[index];
    state.down = false;
    state.releasedThisFrame = true;
    state.repeated = false;

    for (auto& callback : m_releaseCallbacks[index])
    {
        callback();
    }
}

void Input::onKeyPressed(KeyCode code, KeyEventCallback callback)
{
    size_t index = toIndex(code);
    if (index >= KEY_COUNT) return;
    
    m_pressCallbacks[index].push_back(std::move(callback));
}

void Input::onKeyReleased(KeyCode code, KeyEventCallback callback)
{
    size_t index = toIndex(code);
    if (index >= KEY_COUNT) return;
    
    m_releaseCallbacks[index].push_back(std::move(callback));
}

bool Input::isKeyDown(KeyCode key) const
{
    size_t index = toIndex(key);
    if (index >= KEY_COUNT) return false;
    
    return m_keyStates[index].down;
}

bool Input::isKeyUp(KeyCode key) const
{
    return !isKeyDown(key);
}

bool Input::wasKeyPressedThisFrame(KeyCode key) const
{
    size_t index = toIndex(key);
    if (index >= KEY_COUNT) return false;
    
    return m_keyStates[index].pressedThisFrame;
}

bool Input::wasKeyReleasedThisFrame(KeyCode key) const
{
    size_t index = toIndex(key);
    if (index >= KEY_COUNT) return false;
    
    return m_keyStates[index].releasedThisFrame;
}

bool Input::isKeyRepeated(KeyCode key) const
{
    size_t index = toIndex(key);
    if (index >= KEY_COUNT) return false;
    
    return m_keyStates[index].repeated;
}

void Input::endFrame()
{
    for (auto& state : m_keyStates)
    {
        state.pressedThisFrame = false;
        state.releasedThisFrame = false;
        state.repeated = false;
    }
}

size_t Input::toIndex(KeyCode key) const
{
    return static_cast<size_t>(key);
}