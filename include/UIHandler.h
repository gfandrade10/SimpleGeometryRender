#ifndef UIHANDLER_U
#define UIHANDLER_U

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>

namespace RenderApp
{
    class UIHandler
    {
        public:
        UIHandler() { std::cout << "UI App Created\n"; }
        UIHandler(const UIHandler&) = delete;
        UIHandler(UIHandler&&) = delete;
        UIHandler& operator=(const UIHandler&) = delete;
        ~UIHandler();
        
        void Init(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share);
        void Run();

        private:
        int m_WindowWidth;
        int m_WindowHeight;
        bool m_ShouldExit = false;
        GLFWwindow* m_WindowHandler = nullptr;
    };
}

#endif
