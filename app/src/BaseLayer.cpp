#include "BaseLayer.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

BaseLayer::BaseLayer() {
    //glGenVertexArrays(NumVAOs, VAOs);
}

BaseLayer::~BaseLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void BaseLayer::onUpdate(float timestep) {

}

void BaseLayer::onEvent(Event& event) {

}

void BaseLayer::onRender() {

}
