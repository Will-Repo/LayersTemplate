#pragma once
#include "btBulletDynamicsCommon.h"
#include <vector>

struct StaticCollider {
    btTriangleMesh* mesh;
    btBvhTriangleMeshShape* collision;
};

class Physics {
    public:
        Physics() = default;
        // Takes in absolute path, sets up mesh for non-moving object.
        btCollisionShape* addStaticMap(std::string path);
        std::vector<StaticCollider> colliders;
};
