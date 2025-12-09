#pragma once

#include "EverEngineCore/core/Event.h"
#include "EverEngineCore/platform/Input.h"

#include <functional>
#include <memory>

struct GLFWwindow;

class Window
{
public:
    using EventCallbackFn = std::function<void(std::unique_ptr<BaseEvent>)>;

    Window(const unsigned int width, const unsigned int height, const char* title);
    ~Window();

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window& operator=(Window&&) = delete;

    void listenCallbacks();
    bool shouldClose() const;
    void on_update();
    unsigned int get_width() const
    {
        return m_data.width;
    }

    unsigned int get_height() const
    {
        return m_data.height;
    }
    void set_event_callback(const EventCallbackFn& callback);
    GLFWwindow* getHandle() const { return m_window; }

private:
    struct WindowData
    {
        const char* title;
        unsigned int width;
        unsigned int height;
        EventCallbackFn eventCallbackFn;
    };

    int init();
    void shutdown();
    static KeyCode toKeyCode(int glfwKey);
    void pollEvents();
    void swap();

    GLFWwindow* m_window = nullptr;
    WindowData m_data;
};