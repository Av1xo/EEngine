#include "EverEngineCore/platform/Window.h"
#include "EverEngineCore/core/Event.h"
#include "EverEngineCore/core/Log.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include <functional>
#include <memory>


Window::Window(const unsigned int width, const unsigned int height, const char* title)
    : m_data({title, width, height})
{
    int statusCode = init();
    LOG_INFO("WINDOW::INIT::STATUS->{}", statusCode);
}

Window::~Window()
{
    shutdown();
    LOG_INFO("WINDOW::DESTROY");
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
        LOG_CRIT("ERROR::GLFW::INIT");
        return -1;
    }

    m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title, nullptr, nullptr);

    if(!m_window)
    {
        LOG_CRIT("ERROR::WINDOW::CREATE");
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

    glfwSetKeyCallback(m_window, [](GLFWwindow* w, int key, int scancode, int action, int mods)
    {
        Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        WindowData& data = self->m_data;
        
        if(!data.eventCallbackFn)
            return;
        
        if(action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            data.eventCallbackFn(std::make_unique<EventKeyPressed>(Window::toKeyCode(key), scancode, mods, action == GLFW_REPEAT));
        } 
        else if (action == GLFW_RELEASE)
        {
            data.eventCallbackFn(std::make_unique<EventKeyReleased>(Window::toKeyCode(key), scancode, mods));
        }
    });
}

void Window::pollEvents()
{
    glfwPollEvents();
}

KeyCode Window::toKeyCode(int key)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE:        return KeyCode::Esc;
        case GLFW_KEY_F1:            return KeyCode::f1;
        case GLFW_KEY_F2:            return KeyCode::f2;
        case GLFW_KEY_F3:            return KeyCode::f3;
        case GLFW_KEY_F4:            return KeyCode::f4;
        case GLFW_KEY_F5:            return KeyCode::f5;
        case GLFW_KEY_F6:            return KeyCode::f6;
        case GLFW_KEY_F7:            return KeyCode::f7;
        case GLFW_KEY_F8:            return KeyCode::f8;
        case GLFW_KEY_F9:            return KeyCode::f9;
        case GLFW_KEY_F10:           return KeyCode::f10;
        case GLFW_KEY_F11:           return KeyCode::f11;
        case GLFW_KEY_F12:           return KeyCode::f12;

        case GLFW_KEY_DELETE:        return KeyCode::del;
        case GLFW_KEY_HOME:          return KeyCode::home;
        case GLFW_KEY_END:           return KeyCode::end;
        case GLFW_KEY_PAGE_UP:       return KeyCode::pgUp;
        case GLFW_KEY_PAGE_DOWN:     return KeyCode::pgDown;
        case GLFW_KEY_INSERT:        return KeyCode::insert;
        case GLFW_KEY_PAUSE:         return KeyCode::pause;
        case GLFW_KEY_PRINT_SCREEN:  return KeyCode::printScreen;
        case GLFW_KEY_SCROLL_LOCK:   return KeyCode::scrollLock;

        case GLFW_KEY_GRAVE_ACCENT:  return KeyCode::Tld;
        case GLFW_KEY_0:             return KeyCode::k0;
        case GLFW_KEY_1:             return KeyCode::k1;
        case GLFW_KEY_2:             return KeyCode::k2;
        case GLFW_KEY_3:             return KeyCode::k3;
        case GLFW_KEY_4:             return KeyCode::k4;
        case GLFW_KEY_5:             return KeyCode::k5;
        case GLFW_KEY_6:             return KeyCode::k6;
        case GLFW_KEY_7:             return KeyCode::k7;
        case GLFW_KEY_8:             return KeyCode::k8;
        case GLFW_KEY_9:             return KeyCode::k9;
        case GLFW_KEY_MINUS:         return KeyCode::kSub;
        case GLFW_KEY_EQUAL:         return KeyCode::kEq;
        case GLFW_KEY_BACKSPACE:     return KeyCode::Backspace;

        // --- NumPad ---
        case GLFW_KEY_NUM_LOCK:      return KeyCode::nLock;
        case GLFW_KEY_KP_DIVIDE:     return KeyCode::NumDiv;
        case GLFW_KEY_KP_MULTIPLY:   return KeyCode::NumMul;
        case GLFW_KEY_KP_SUBTRACT:   return KeyCode::NumSub;
        case GLFW_KEY_KP_ADD:        return KeyCode::NumAdd;
        case GLFW_KEY_KP_ENTER:      return KeyCode::NumEnter;
        case GLFW_KEY_KP_DECIMAL:    return KeyCode::NumDot;
        case GLFW_KEY_KP_0:          return KeyCode::Num0;
        case GLFW_KEY_KP_1:          return KeyCode::Num1;
        case GLFW_KEY_KP_2:          return KeyCode::Num2;
        case GLFW_KEY_KP_3:          return KeyCode::Num3;
        case GLFW_KEY_KP_4:          return KeyCode::Num4;
        case GLFW_KEY_KP_5:          return KeyCode::Num5;
        case GLFW_KEY_KP_6:          return KeyCode::Num6;
        case GLFW_KEY_KP_7:          return KeyCode::Num7;
        case GLFW_KEY_KP_8:          return KeyCode::Num8;
        case GLFW_KEY_KP_9:          return KeyCode::Num9;

        case GLFW_KEY_TAB:           return KeyCode::Tab;
        case GLFW_KEY_CAPS_LOCK:     return KeyCode::CapsLock;
        case GLFW_KEY_LEFT_SHIFT:    return KeyCode::LShift;
        case GLFW_KEY_RIGHT_SHIFT:   return KeyCode::RShift;
        case GLFW_KEY_LEFT_CONTROL:  return KeyCode::LCtrl;
        case GLFW_KEY_RIGHT_CONTROL: return KeyCode::RCtrl;
        case GLFW_KEY_LEFT_ALT:      return KeyCode::LAlt;
        case GLFW_KEY_RIGHT_ALT:     return KeyCode::RAlt;
        case GLFW_KEY_LEFT_SUPER:    return KeyCode::LSuper;
        case GLFW_KEY_RIGHT_SUPER:   return KeyCode::RSuper;
        case GLFW_KEY_MENU:          return KeyCode::Menu;

        case GLFW_KEY_ENTER:         return KeyCode::Enter;
        case GLFW_KEY_SPACE:         return KeyCode::Space;

        case GLFW_KEY_UP:            return KeyCode::ArrowUp;
        case GLFW_KEY_DOWN:          return KeyCode::ArrowDown;
        case GLFW_KEY_LEFT:          return KeyCode::ArrowLeft;
        case GLFW_KEY_RIGHT:         return KeyCode::ArrowRight;

        case GLFW_KEY_A: return KeyCode::A;
        case GLFW_KEY_B: return KeyCode::B;
        case GLFW_KEY_C: return KeyCode::C;
        case GLFW_KEY_D: return KeyCode::D;
        case GLFW_KEY_E: return KeyCode::E;
        case GLFW_KEY_F: return KeyCode::F;
        case GLFW_KEY_G: return KeyCode::G;
        case GLFW_KEY_H: return KeyCode::H;
        case GLFW_KEY_I: return KeyCode::I;
        case GLFW_KEY_J: return KeyCode::J;
        case GLFW_KEY_K: return KeyCode::K;
        case GLFW_KEY_L: return KeyCode::L;
        case GLFW_KEY_M: return KeyCode::M;
        case GLFW_KEY_N: return KeyCode::N;
        case GLFW_KEY_O: return KeyCode::O;
        case GLFW_KEY_P: return KeyCode::P;
        case GLFW_KEY_Q: return KeyCode::Q;
        case GLFW_KEY_R: return KeyCode::R;
        case GLFW_KEY_S: return KeyCode::S;
        case GLFW_KEY_T: return KeyCode::T;
        case GLFW_KEY_U: return KeyCode::U;
        case GLFW_KEY_V: return KeyCode::V;
        case GLFW_KEY_W: return KeyCode::W;
        case GLFW_KEY_X: return KeyCode::X;
        case GLFW_KEY_Y: return KeyCode::Y;
        case GLFW_KEY_Z: return KeyCode::Z;

        case GLFW_KEY_LEFT_BRACKET:   return KeyCode::LBracket;
        case GLFW_KEY_RIGHT_BRACKET:  return KeyCode::RBracket;
        case GLFW_KEY_BACKSLASH:      return KeyCode::Backslash;
        case GLFW_KEY_SEMICOLON:      return KeyCode::Semicolon;
        case GLFW_KEY_APOSTROPHE:     return KeyCode::Apostrophe;
        case GLFW_KEY_COMMA:          return KeyCode::Comma;
        case GLFW_KEY_PERIOD:         return KeyCode::Dot;
        case GLFW_KEY_SLASH:          return KeyCode::Slash;

        default:
            return KeyCode::Unknown;
    }
}

Window::ProcLoader Window::getProcLoader()
{
    return [](const char* name) -> void* {
        return (void*)glfwGetProcAddress(name);
    };
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