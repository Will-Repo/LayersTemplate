#pragma once
#include "LogicThread.h"
#include "Window.h"
#include <map>
#include <memory>

class LogicThreadManager {
    public:
        void addLayer(Layer* layer);
        void startThreads();
    private:
        std::map<int, std::unique_ptr<LogicThread>> threadGroups;
};
