#pragma once
#include "UpdateThread.h"
#include "RenderingThread.h"
#include "InputThread.h"
#include "Window.h"
#include <map>
#include <memory>

class ThreadManager {
    public:
        ThreadManager();

        //naming: add - action - passed type
        //Layer updating - layers can be assigned to any thread group, no limitations except sticking to a single thread group.
        void addUpdateLayer(Layer* layer);
        //Window rendering - layers must be assigned to the thread group for their window, as the context for that layer will only be current on a single thread group.
        void addRenderingWindow(Window* window, FilePaths* filePaths);
        //Windows' input handling - recieves event objects from callbacks and polling on main thread, multiple windows can be assigned to a thread group, but each window must be assigned to only one thread group. Thread group identifies window event belongs to, and passes event down layers until its been handled.
        void addInputWindow(Window* window);

        void startAllThreads();

        void startLayerUpdating();
        void startWindowRendering();
        void startHandlingInputs();
    private:
        //Layer updating
        std::map<int, std::unique_ptr<UpdateThread>> updateThreadGroups;
        //Window rendering
        std::map<int, std::unique_ptr<RenderingThread>> renderingThreadGroups;
        //Windows' input handling - input queues.
        std::map<int, std::unique_ptr<InputThread>> inputThreadGroups;
};
