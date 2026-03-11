#pragma once
#include "Thread.h"
#include "Window.h"
#include <map>
#include <memory>

class ThreadManager {
    public:
        void startWindow(Window* window);
    private:
        std::map<int, std::unique_ptr<Thread>> threadGroups;
};
