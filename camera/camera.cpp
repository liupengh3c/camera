// camera.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL.h>
// GLFW
#include <GLFW/glfw3.h>
using namespace std;
const int WIDTH = 800, HEIGHT = 600;
int main()
{
    int width, height;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "camera study", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "failed to create glfw window" << endl;
        glfwTerminate();
        return -1;
    }

    // 设置上下文
    glfwMakeContextCurrent(window);

    // init glew
    glewExperimental = GL_TRUE;
    glewInit();

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Shader,顶点着色器
    const GLchar* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "layout (location = 2) in vec2 texCoord;\n"

        "out vec4 vertexColor;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position, 1.0);\n"
        "vertexColor = vec4(color,1.0f);\n"
        "TexCoord = vec2(texCoord.x, 1 - texCoord.y);\n"
        "}\0";

    // create shader && bind source && compile src
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    }

    // fragment shader
    const GLchar* fragShaderSource = "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "in vec2 TexCoord;\n"
        "out vec4 color;\n"
        // Texture samplers
        "uniform sampler2D ourTexture1;\n"
        "uniform sampler2D ourTexture2;\n"
        "void main()\n"
        "{\n"
        // Linearly interpolate between both textures (second texture is only slightly combined)
        "color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);\n"
        "}\n";

    GLuint fragmentShaser = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaser, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShaser);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShaser);

    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaser);

    GLfloat vertices[] = {
       -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f
    };

    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    GLuint EBO, VBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
