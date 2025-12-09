#pragma once

#include "EverEngineCore/platform/Keyboard.h"
#include "EverEngineCore/core/Event.h"
#include <array>
#include <functional>
#include <vector>

class Input
{
public:
    using KeyEventCallback = std::function<void()>;

    void init(EventDispatcher& dispatcher);

    void onKeyPressed(KeyCode code, KeyEventCallback callback);
    void onKeyReleased(KeyCode code, KeyEventCallback callback);

    bool isKeyDown(KeyCode key) const;
    bool isKeyUp(KeyCode key) const;

    bool wasKeyPressedThisFrame(KeyCode key) const;
    bool wasKeyReleasedThisFrame(KeyCode key) const;

    bool isKeyRepeated(KeyCode key) const;

    void endFrame();

private:
    struct KeyState
    {
        bool down = false;
        bool pressedThisFrame = false;
        bool releasedThisFrame = false;
        bool repeated = false;
    };

    static constexpr size_t KEY_COUNT = static_cast<size_t>(KeyCode::Slash) + 1;
    
    std::array<KeyState, KEY_COUNT> m_keyStates{};
    
    std::array<std::vector<KeyEventCallback>, KEY_COUNT> m_pressCallbacks;
    std::array<std::vector<KeyEventCallback>, KEY_COUNT> m_releaseCallbacks;

    void handleKeyPressed(EventKeyPressed& e);
    void handleKeyReleased(EventKeyReleased& e);
    
    size_t toIndex(KeyCode key) const;
};