#include <EverEngineCore/core/Engine.h>
#include <iostream>
#include <memory>

class SandBox: public Engine
{
public:
    virtual void set_eventCallback() override
    {
        getDispatcher().add_event_listener<EventWindowResize>(
            [](EventWindowResize& event)
            {
                std::cout << "Window resize: " << event.width << "x" << event.height << std::endl; 
            }
        );
    }
    virtual void on_update() override
    {

    }
};

int main()
{
    std::cout << "Engine run" << std::endl;
    auto sandbox = std::make_unique<SandBox>();
    int returnCode = sandbox->init(1024, 720, "SandBox");
    if (returnCode){
        return returnCode;
    }
    returnCode = sandbox->run();

    std::cin.get();
    return returnCode;
}