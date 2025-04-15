#include <exception>
#include <iostream>
#include <memory>
#include <fmt/core.h>
#include "UIHandler.h"

int main()
{
    std::cout << "Starting UI application.\n";

    try
    {
        auto app = std::make_unique<RenderApp::UIHandler>();
        app->Init(1920, 1080, "Simple Geometry App", nullptr, nullptr);
        app->Run();
    }
    catch(std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "Program finished with success!\n";
    return 0;
}
