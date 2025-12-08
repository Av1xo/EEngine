#pragma once

class Engine
{
private:
    bool m_running = true;
public:
    Engine();
    ~Engine();

    void run();
};
