#include "Layer.h"

class BaseLayer : public Layer {
    public:
        BaseLayer();
        ~BaseLayer();
        void onUpdate(float timestep) override;
        void onEvent(Event& event) override;
        void onRender() override;
};
