#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(int width, int height, const char* title)
{
    if(!glfwInit())
    {
        std::cerr << "FAIL::INIT::GLFW";
        return;
    }

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if(!m_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::pollEvents()
{
    glfwPollEvents();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}