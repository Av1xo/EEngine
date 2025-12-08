#include "EverEngineCore/platform/Window.h"
#include "EverEngineCore/core/Event.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include <functional>
#include <memory>


Window::Window(const unsigned int width, const unsigned int height, const char* title)
    : m_data({title, width, height})
{
    int statusCode = init();
}

Window::~Window()
{
    shutdown();
}

void Window::shutdown()
{
    if (m_window) glfwDestroyWindow(m_window);
    glfwTerminate();
}

int Window::init()
{
    if(!glfwInit())
    {
        std::cerr << "FAIL::INIT::GLFW";
        return -1;
    }

    m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title, nullptr, nullptr);

    if(!m_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, this);
    listenCallbacks();

    return 0;
}

void Window::set_event_callback(const EventCallbackFn& callback)
{
    m_data.eventCallbackFn = callback;
}

void Window::listenCallbacks()
{
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* w, int width, int height)
    {
        Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        WindowData& data = self->m_data;
        data.width = width;
        data.height = height;

        data.eventCallbackFn(std::make_unique<EventWindowResize>(width, height));
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double xPos, double yPos)
    {
        Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        WindowData& data = self->m_data;
        
        data.eventCallbackFn(std::make_unique<EventMouseMoved>(xPos, yPos));
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* w)
    {
        Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        WindowData& data = self->m_data;

        data.eventCallbackFn(std::make_unique<EventWindowClose>());
    });

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
    });
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::on_update()
{
    pollEvents();
    swap();
}

void Window::swap()
{
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}