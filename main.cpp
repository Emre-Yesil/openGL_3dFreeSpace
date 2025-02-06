#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

const int WIN_WIDTH = 1800;
const int WIN_HEIGHT = 1600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void setTexData(const char* fileName);

int main() {
    //set openGL glfw glad
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //set frames and depth
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //object verticies
    float groundVertices[] = {
        // positions          // texCoords
         1000.0f,  0.0f,  1000.0f,  10.0f, 0.0f,
         1000.0f,  0.0f, -1000.0f,  10.0f, 10.0f,
        -1000.0f,  0.0f, -1000.0f,  0.0f, 10.0f,
        -1000.0f,  0.0f,  1000.0f,  0.0f, 0.0f
    };
    float wallVerticies[] = {
        // positions                    // texCoords
         1000.0f,  0.0f,    1000.0f,      10.0f, 0.0f,
        -1000.0f,  0.0f,    1000.0f,      0.0f, 0.0f,
        -1000.0f,  1000.0f, 1000.0f,      0.0f, 10.0f,
         1000.0f,  1000.0f, 1000.0f,      10.0f, 10.0f
    };
    unsigned int indices1[] = {
        0, 1, 2,
        2, 3, 0 };

    //bind objects
    unsigned int VAOs[3], VBOs[3], EBOs[3];
        //the ground
    glGenVertexArrays(1, &VAOs[0]);
    glGenBuffers(1, &VBOs[0]);
    glGenBuffers(1, &EBOs[0]);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

        //the wall
    glGenVertexArrays(1, &VAOs[1]);
    glGenBuffers(1, &VBOs[1]);
    glGenBuffers(1, &EBOs[1]);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVerticies), wallVerticies, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //textures
    unsigned int textures[3];
    glGenTextures(3, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    setTexData("stone.jpg");
    
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    setTexData("wood.jpg");
    
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    setTexData("grass.jpg");

    //objects
    shader myShader("vShader.vert", "fShader.frag");

    camera myCamera(cameraPos, cameraFront, cameraUp, (float)(WIN_WIDTH / 2), (float)(WIN_WIDTH / 2), window, cameraFront);
    
    //main loop
    while (!glfwWindowShouldClose(window)) {
        //background color and depth
        glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //set textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        myShader.setInt("textures[0]", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        myShader.setInt("textures[1]", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        myShader.setInt("textures[2]", 2);
    
        //processInput(window);
            //wasd
        myCamera.processInput(window);
            //mouse
        if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
        {
            glfwSetCursorPosCallback(window, myCamera.mouseCallbackWrapper);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, NULL);
        }

        //use shader programe
        myShader.use();

        glBindVertexArray(VAOs[0]);

        //transformations
        glm::mat4 view = myCamera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(WIN_WIDTH) / WIN_HEIGHT, 0.1f, 10000.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -100.0f, -10.0f));
        //model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
     
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);
        myShader.setMat4("model", model);

        //draw element
        myShader.setInt("selTex", 2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAOs[1]);
        myShader.setInt("selTex", 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //delete dynamic memory
    glDeleteVertexArrays(1, &VAOs[0]);
    glDeleteBuffers(1, &VBOs[0]);
    glDeleteBuffers(1, &EBOs[0]);
    glDeleteTextures(2, textures);

    glfwTerminate();
    return 0;
}

//frame buffer size function
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//texture set function
void setTexData(const char* fileName) {
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture: " << fileName << std::endl;
    }
    stbi_image_free(data);
    data = NULL;
}
