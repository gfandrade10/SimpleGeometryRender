#include <exception>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "UIHandler.h"
#include "Utilities.h"
#include "RenderObject2D.h"
#include "Objects.h"

namespace RenderApp
{
    void UIHandler::Init(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
    {
        try
        {
            if (!glfwInit())
                throw std::runtime_error("Failed to load init\n");

            m_WindowWidth = width;
            m_WindowHeight = height;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_MAXIMIZED, true);

            // Create window
            m_WindowHandler = glfwCreateWindow(width, height, title, monitor, share);
            if (!m_WindowHandler) 
            {
                glfwTerminate();
                throw std::runtime_error("Failed to load window\n");
            }

            glfwMakeContextCurrent(m_WindowHandler);
            glfwSetFramebufferSizeCallback(m_WindowHandler, [](GLFWwindow* window, int width, int height) ->void
                { glViewport(0, 0, width, height); });

            // Load OpenGL using GLAD
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
            {
                this->~UIHandler();
                throw std::runtime_error("Failed to load GLAD\n");
            }

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark(); // Dark theme

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(m_WindowHandler, true);
            ImGui_ImplOpenGL3_Init("#version 330");
        }
        catch(const std::exception& e)
        {
            std::cerr << "[UIHandler::Init] Exception: " << e.what();
        }
    }

    void UIHandler::Run()
    {
        try
        {
            auto io = ImGui::GetIO();
            io.Fonts->AddFontDefault();
            ImFont* ButtonFont = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/calibri.ttf", 22.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

            RenderApp::RenderObject2D Triangle = CreateTriangle();
            Triangle.Init(512, 512);

            glEnable(GL_DEPTH_TEST);

            while (!glfwWindowShouldClose(m_WindowHandler) && !m_ShouldExit)
            {
                glfwPollEvents();
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                ImVec2 WindowSize (m_WindowWidth - 16, m_WindowHeight - 70);
                ImGui::SetNextWindowSize(WindowSize);
                ImGui::SetNextWindowPos(ImVec2(8, 10));
                ImGui::Begin("Hello, ImGui!", nullptr, ImGuiWindowFlags_NoResize
                                                     | ImGuiWindowFlags_NoTitleBar
                                                     | ImGuiWindowFlags_NoMove);

                ImGui::PushFont(ButtonFont);

                //Text to be displayed
                std::wstring ws = L"Aplicação Gráfica usando ImGui + GLFW + OpenGL!";
                std::string text = WStringToUtf8(ws);
                ImVec2 TextSize = ImGui::CalcTextSize(text.c_str());
                
                //Render and display geometry (triangle) in correct position
                Triangle.Render();
                ImGui::SetCursorPos(ImVec2(WindowSize.x/2 - (float)Triangle.m_ImgWidth/2,
                                        WindowSize.y/2 - (float)Triangle.m_ImgHeight/2 - TextSize.y));

                ImGui::Image(static_cast<ImTextureID>(Triangle.GetTexture()), ImVec2(Triangle.m_ImgWidth, Triangle.m_ImgHeight));

                //Adjust position for text display
                ImVec2 TextAfterImgPos = ImGui::GetCursorPos();
                ImGui::SetCursorPos(ImVec2(TextAfterImgPos.x + WindowSize.x/2 - TextSize.x/2 - 5.0f,
                                           TextAfterImgPos.y + TextSize.y - 10.0f));

                ImGui::Text("%s", text.c_str());

                ImVec2 ButtonSize(80, 30);
                ImGui::SetCursorPos(ImVec2(WindowSize.x - 90, WindowSize.y - 40));
                if(ImGui::Button("EXIT", ButtonSize))
                {
                    m_ShouldExit = true;
                }

                ImGui::PopFont();
                ImGui::End();
                ImGui::Render();

                glViewport(0, 0, Triangle.m_ImgWidth, Triangle.m_ImgHeight);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                glfwSwapBuffers(m_WindowHandler);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "[UIHandler::Init] Exception: " << e.what();
        }
    }

    UIHandler::~UIHandler()
    {
        std::cout << "UI App Deleted\n";
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(m_WindowHandler);
        glfwTerminate();
    }
}
