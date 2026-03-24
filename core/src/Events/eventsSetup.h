#include <memory>
#include <vector>
#include "Window.h"

void setCallbacks(std::vector<std::shared_ptr<Window>>& windowStack);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_enter_callback(GLFWwindow* window, int entered);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
