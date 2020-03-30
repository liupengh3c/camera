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

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

using namespace std;
const int WIDTH = 800, HEIGHT = 600;
int main()
{
    int width, height;
    int textureWidth, textureHeight, nrChannels;
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

        "out vec2 TexCoord;\n"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        "void main()\n"
        "{\n"
        "gl_Position =  projection * view * model * vec4(position, 1.0f);\n"
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

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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


    GLuint VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // 加载shader
    GLuint texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // 采样设置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 多级渐远纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 纹理图像加载
    //unsigned char* image = stbi_load("texture\\container.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    unsigned char* image = SOIL_load_image("texture\\container.jpg", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
   
    //stbi_image_free(image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 采样设置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 多级渐远纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 纹理图像加载
    //image = stbi_load("texture\\awesomeface.png", &textureWidth, &textureHeight, &nrChannels, 0);
    image = SOIL_load_image("texture\\awesomeface.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
   
    //stbi_image_free(image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"),0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1);

        glUseProgram(shaderProgram);

        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        //model = glm::rotate(model, 40.0f * (GLfloat)glfwGetTime(), glm::vec3(0.5f, 1.0f, 1.0f));
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        GLfloat radius = 10.0f;
        GLfloat camX = sin(glfwGetTime()) * radius;
        GLfloat camZ = cos(glfwGetTime()) * radius;
        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

        //GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + 40.0f;
            model = glm::rotate(model, angle * GLfloat(glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
            GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glBindVertexArray(0);

        glfwSwapBuffers(window);

       
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
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
