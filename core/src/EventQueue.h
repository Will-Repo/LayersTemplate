// 1 event queue per window, uses callbacks to handle events, which are handled immediately by the layer and marked as handled to stop event propogation.
class EventQueue {
    public:
        EventQueue(Window* windowPtr) : window(windowPtr);
    private:
        Window* window;
};

