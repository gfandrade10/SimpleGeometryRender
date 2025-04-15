#include "RenderObject2D.h"
#include <vector>
#include <glad/glad.h>

RenderApp::RenderObject2D CreateTriangle()
{
    std::vector<float> vertices = {
         0.8f,  0.8f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top right
        -0.8f,  0.8f, 0.0f,  0.0f, 1.0f, 0.0f,  // Top left
         0.0f, -0.8f, 0.0f,  0.0f, 0.0f, 1.0f   // Bottom center
    };

    std::vector<GLuint> indices = {2, 1, 0};

    RenderApp::RenderObject2D Triangle(vertices, indices);

    return Triangle;
};
