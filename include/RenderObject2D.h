#ifndef RENDEROBJECT2D_H
#define RENDEROBJECT2D_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RenderApp
{
    class RenderObject2D
    {
        public:
        RenderObject2D() = default;
        RenderObject2D(RenderObject2D&& obj) noexcept;
        RenderObject2D(RenderObject2D const&) = delete;
        RenderObject2D& operator=(RenderObject2D const&) = delete;

        RenderObject2D& operator=(RenderObject2D&& obj) noexcept
        {
            if(this != &obj)
            {
                m_VertexBuffer = std::move(obj.m_VertexBuffer);
                m_VertexIndices = std::move(obj.m_VertexIndices);
                m_VAO = obj.m_VAO;
                m_VBO = obj.m_VBO;
                m_EBO = obj.m_EBO;
                m_FBO = obj.m_FBO;
                m_RBO = obj.m_RBO;
                m_Texture = obj.m_Texture;

                obj.m_VAO = 0;
                obj.m_VBO = 0;
                obj.m_EBO = 0;
                obj.m_FBO = 0;
                obj.m_RBO = 0;
                obj.m_Texture = 0;
            }
            return *this;
        }

        RenderObject2D(std::vector<float> const& vertex, std::vector<GLuint> const& indices);

        void SetVertices(std::vector<float> const& vertex);
        void SetIndices(std::vector<GLuint> const& indices);
        GLuint GetTexture() const;
        GLuint CompileShader(GLuint shaderType, char const* source);
        GLuint LinkShaders(GLuint vertexShader, GLuint fragmentShader);
        void Init(int ImgWidth, int ImgHeight);
        void Render();

        ~RenderObject2D();

        int m_ImgWidth = 0, m_ImgHeight = 0;

        private:
        std::vector<float> m_VertexBuffer;
        std::vector<GLuint> m_VertexIndices;
        GLuint m_VAO = 0;
        GLuint m_VBO = 0;
        GLuint m_EBO = 0;
        GLuint m_FBO = 0;
        GLuint m_RBO = 0;
        GLuint m_Texture = 0;
        GLuint m_ShaderProgram = 0;
    };
}

#endif
