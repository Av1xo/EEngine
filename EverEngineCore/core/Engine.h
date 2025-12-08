#pragma once

#include "Window.h"
#include <memory>


class Engine
{
private:
    std::unique_ptr<Window> m_window;
public:
    Engine();
    ~Engine();

    void run();
};
