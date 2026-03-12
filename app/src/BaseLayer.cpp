#include "BaseLayer.h"
#include <iostream>

BaseLayer::BaseLayer() {

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
