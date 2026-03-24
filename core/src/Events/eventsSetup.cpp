#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "eventsSetup.h"
#include <iostream>

void setCallbacks(std::vector<std::shared_ptr<Window>>& windowStack) {
    // Set basic callbacks for all windows. Each callback will pass the respecitve event object to the window's event queue.
    for (const auto& window : windowStack) {
        glfwMakeContextCurrent(window->getWindow());

        glfwSetKeyCallback(window->getWindow(), key_callback);
        //glfwSetCursorPosCallback(window, cursor_position_callback); // Can just get cursor position through query.
        glfwSetCursorEnterCallback(window->getWindow(), cursor_enter_callback);
        glfwSetMouseButtonCallback(window->getWindow(), mouse_button_callback);
        glfwSetScrollCallback(window->getWindow(), scroll_callback);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        std::cout << "Hello world" << std::endl;
    }
}

void cursor_enter_callback(GLFWwindow* window, int entered) {
    if (entered) {
        // The cursor entered the content area of the window
    } else {
        // The cursor left the content area of the window
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {

    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}

// Function that takes in window pointer and event, and handles thread synchronisation/locks and puts event in windows queue.
//void enqueueEvent(std::weak_ptr<Window> window, 
