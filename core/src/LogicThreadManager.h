#pragma once
#include "LogicThread.h"
#include "Window.h"
#include <map>
#include <memory>

class LogicThreadManager {
    public:
        void startWindow(Window* window);
        void startThreads();
    private:
        std::map<int, std::unique_ptr<LogicThread>> threadGroups;
};
