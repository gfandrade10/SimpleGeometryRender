#include "RenderObject2D.h"
#include "Utilities.h"
#include "imgui.h"
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RenderApp
{
    RenderObject2D::RenderObject2D(RenderObject2D&& obj) noexcept
        : m_VertexBuffer(std::move(obj.m_VertexBuffer)),
        m_VertexIndices(std::move(obj.m_VertexIndices)),
        m_VAO(obj.m_VAO),
        m_VBO(obj.m_VBO),
        m_EBO(obj.m_EBO),
        m_FBO(obj.m_FBO),
        m_RBO(obj.m_RBO),
        m_Texture(obj.m_Texture)
    {
        obj.m_VAO = 0;
        obj.m_VBO = 0;
        obj.m_EBO = 0;
        obj.m_FBO = 0;
        obj.m_RBO = 0;
        obj.m_Texture = 0;
    }

    RenderObject2D::RenderObject2D(std::vector<float> const& vertex, std::vector<GLuint> const& indices)
    {
        if(!vertex.empty() && !indices.empty())
        {
            m_VertexBuffer = vertex;
            m_VertexIndices = indices;
        }
    }

    void RenderObject2D::SetVertices(std::vector<float> const& vertex)
    {
        m_VertexBuffer = vertex;
    }

    void RenderObject2D::SetIndices(std::vector<GLuint> const& indices)
    {
        m_VertexIndices = indices;
    }

    GLuint RenderObject2D::GetTexture() const
    {
        return this->m_Texture;
    }

    GLuint RenderObject2D::CompileShader(GLuint shaderType, char const* source)
    {
        GLuint Shader = glCreateShader(shaderType);
        glShaderSource(Shader, 1, &source, nullptr);
        glCompileShader(Shader);
        return Shader;
    }

    GLuint RenderObject2D::LinkShaders(GLuint vertexShader, GLuint fragmentShader)
    {
        GLuint ShaderProgram = glCreateProgram();
        glAttachShader(ShaderProgram, vertexShader);
        glAttachShader(ShaderProgram, fragmentShader);
        glLinkProgram(ShaderProgram);
        return ShaderProgram;
    }

    void RenderObject2D::Init(int ImgWidth, int ImgHeight)
    {
        m_ImgWidth = ImgWidth;
        m_ImgHeight = ImgHeight;

        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
        if (m_EBO) glDeleteBuffers(1, &m_EBO);
        if (m_FBO) glDeleteFramebuffers(1, &m_FBO);
        if (m_RBO) glDeleteRenderbuffers(1, &m_RBO);
        if (m_Texture) glDeleteTextures(1, &m_Texture);

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glGenFramebuffers(1, &m_FBO);
        glGenRenderbuffers(1, &m_RBO);

        //Vertex | Indices arrays - map to GPU        
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_VertexBuffer.size() * sizeof(float), m_VertexBuffer.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_VertexIndices.size() * sizeof(GLuint), m_VertexIndices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //map texture to GPU
        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImgWidth, ImgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //map fbo to GPU
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

        //map rbo to GPU
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ImgWidth, ImgHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

        std::string VertexShaderSrc = GLSLtoString("F:/Codes/SimpleGeometry/glsl/VertexShader.glsl");
        std::string FragmentShaderSrc = GLSLtoString("F:/Codes/SimpleGeometry/glsl/FragmentShader.glsl");

        GLuint VShader = CompileShader(GL_VERTEX_SHADER, VertexShaderSrc.c_str());
        GLuint FShader = CompileShader(GL_FRAGMENT_SHADER, FragmentShaderSrc.c_str());
        m_ShaderProgram = LinkShaders(VShader, FShader);

        glDeleteShader(VShader);
        glDeleteShader(FShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(0);
    }

    void RenderObject2D::Render()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glViewport(0, 0, m_ImgWidth, m_ImgHeight);

        ImVec4 clearColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_ShaderProgram);
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_VertexIndices.size()), GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);        
    }

    RenderObject2D::~RenderObject2D()
    {
        if(m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if(m_VBO) glDeleteBuffers(1, &m_VBO);
        if(m_EBO) glDeleteBuffers(1, &m_EBO);
        if(m_FBO) glDeleteFramebuffers(1, &m_FBO);
        if(m_RBO) glDeleteRenderbuffers(1, &m_RBO);
        if(m_Texture) glDeleteTextures(1, &m_Texture);
        if(m_ShaderProgram) glDeleteProgram(m_ShaderProgram);

        m_VAO = 0;
        m_VBO = 0;
        m_EBO = 0;
        m_FBO = 0;
        m_RBO = 0;
        m_Texture = 0;
        m_ShaderProgram = 0;
    }
}
