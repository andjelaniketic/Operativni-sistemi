#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//void update(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//string u vise linija, fragment shader
const char *vertexShaderSource = R"S(
#version 330 core
//3d vector
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aOff;

void main()
{
    gl_Position = vec4(aPos.x + aOff.x, aPos.y + aOff.y, aPos.z, 1.0);
}
)S";

const char *fragmentShaderSource = R"S(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)S";

int main() {
    glfwInit();
    //OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "hello_window", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create a window!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    //shaderi
    unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success = 0;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    unsigned shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //trougao pa kvadrat od 2 trougla
    float vertices[] = {
            //prvi trougao
            0.5f, 0.5f, 0.0f, 0.2f, 0.2f,//gore desno
            0.5f, -0.5f, 0.0f, 0.2f, 0.2f,//dole desno

            -0.5f, -0.5f, 0.0f, 0.2f, 0.2f, //dole levo
            -0.5f, 0.5f, 0.0f, 0.2f, 0.2f //gore levo
    };
    //vertex0       vertex1         vertex2
    //x,y,z,ox,oy   x,y,z,ox,oy     x,y,z,ox,oy
    unsigned indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    //vertex buffer object - iz rama ucitava na graficku
    //vertex array object - kaze grafickoj sta ti podaci koje ucitamo znace
    //element buffer object

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO); //kazemo sta znace ovi podaci koje saljemo

    glGenBuffers(1, &VBO); //kreiranje objekta
    glGenBuffers(1, &EBO); //kreiranje objekta

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //aktivacija objekta
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //salje na graficku

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //aktivacija, obavezno nakon vertex array aktivacije
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float))); //offset
    glEnableVertexAttribArray(0); //aktivacija attribpointera
    glEnableVertexAttribArray(1); //aktivacija attribpointera za offset

    glBindBuffer(GL_ARRAY_BUFFER, 0); //razvezivanje
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //da se vide samo linije
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //petlja za renderovanje
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); //dovoljno za crtanje aktivirati vao

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_G && action == GLFW_PRESS){
        std::cerr << "G\n";
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    //R
    if (key == GLFW_KEY_R && action == GLFW_PRESS){
        glClearColor(1.0, 0.0, 0.0, 1.0);
    }
    //G
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        glClearColor(0.0, 1.0, 0.0, 1.0);
    }
    //B
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        glClearColor(0.0, 0.0, 1.0, 1.0);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}
