#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>


class camera {
public:
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX ,lastY;
    glm::vec3 front;
    GLFWwindow* win;
    bool firstMouse;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float x, float y, GLFWwindow* win, glm::vec3 f)
        : cameraPos(pos), cameraFront(front), cameraUp(up), lastX(x), lastY(y), front(f),
            firstMouse(true) {
        glfwSetWindowUserPointer(win, this);
    };

    glm::mat4 getViewMatrix() {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void processInput(GLFWwindow* window) {
        float cameraSpeed = 0.1f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }

    void mouseCallBack(GLFWwindow* window, double xPos, double yPos) 
    {
        if (firstMouse) {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }
        float xOffset = xPos - lastX;
        float yOffset = yPos - lastY;
        lastX = xPos;
        lastY = yPos;
        const float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        yaw += xOffset;
        pitch += yOffset;
        if (pitch > 90.0f)
            pitch = 90.0f;
        if (pitch < -90.0f)
            pitch = -90.0f;
        
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = -sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }  
    // Static callback wrapper
    static void mouseCallbackWrapper(GLFWwindow* window, double xPos, double yPos) {
        camera* cam = static_cast<camera*>(glfwGetWindowUserPointer(window));
        if (cam) {
            cam->mouseCallBack(window, xPos, yPos);
        }
    }
};



#endif // !CAMERA_H