#pragma once

struct GLFWwindow;

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    void pollEvents();
    bool shouldClose() const;

    GLFWwindow* getHandle() const { return m_window; }

private:
    GLFWwindow* m_window = nullptr;
};