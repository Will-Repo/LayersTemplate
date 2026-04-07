#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "eventsSetup.h"
#include <iostream>
#include "Event.h"
#include "KeyEvent.h"
#include "CursorWindowEvent.h"
#include "MouseButtonEvent.h"
#include "ScrollEvent.h"

void setCallbacks(std::vector<std::shared_ptr<Window>>& windowStack) {
    // Set basic callbacks for all windows. Each callback will pass the respecitve event object to the window's event queue.
    for (const auto& window : windowStack) {
        glfwMakeContextCurrent(window->getWindow());

        // Associate a pointer to the outer window object from the glfw window, allowing it to pass an event in the callback.
        auto userData = new std::weak_ptr<Window>(window);
        glfwSetWindowUserPointer(window->getWindow(), userData);

        glfwSetKeyCallback(window->getWindow(), key_callback);
        //glfwSetCursorPosCallback(window, cursor_position_callback); // Can just get cursor position through query.
        glfwSetCursorEnterCallback(window->getWindow(), cursor_enter_callback);
        glfwSetMouseButtonCallback(window->getWindow(), mouse_button_callback);
        glfwSetScrollCallback(window->getWindow(), scroll_callback);
    }
}

void key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
    auto windowPtr = static_cast<std::weak_ptr<Window>*>(glfwGetWindowUserPointer(glfwWindow));
    if (auto window = windowPtr->lock()) {
        auto keyEvent = std::make_shared<KeyEvent>(key, action);
        window->enqueueEvent(std::move(keyEvent));
    }
}

void cursor_enter_callback(GLFWwindow* glfwWindow, int entered) {
    if (entered) {
        // The cursor entered the content area of the window
    } else {
        // The cursor left the content area of the window
    }

    auto windowPtr = static_cast<std::weak_ptr<Window>*>(glfwGetWindowUserPointer(glfwWindow));
    if (auto window = windowPtr->lock()) {
        auto cursorWindowEvent = std::make_shared<CursorWindowEvent>();
        window->enqueueEvent(std::move(cursorWindowEvent));
    }
}

void mouse_button_callback(GLFWwindow* glfwWindow, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        auto windowPtr = static_cast<std::weak_ptr<Window>*>(glfwGetWindowUserPointer(glfwWindow));
        if (auto window = windowPtr->lock()) {
            auto mouseButtonEvent = std::make_shared<MouseButtonEvent>();
            window->enqueueEvent(std::move(mouseButtonEvent));
        }
    }
}

void scroll_callback(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
    auto windowPtr = static_cast<std::weak_ptr<Window>*>(glfwGetWindowUserPointer(glfwWindow));
    if (auto window = windowPtr->lock()) {
        auto scrollEvent = std::make_shared<ScrollEvent>();
        window->enqueueEvent(std::move(scrollEvent));
    }
}
