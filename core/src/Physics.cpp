#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Physics.h"
#include "btBulletDynamicsCommon.h"
#include <iostream>
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"

void processMesh(aiMesh* mesh, const aiScene* scene, btTriangleMesh* colliderMesh) {
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3) continue;

        aiVector3D a = mesh->mVertices[face.mIndices[0]];
        aiVector3D b = mesh->mVertices[face.mIndices[1]];
        aiVector3D c = mesh->mVertices[face.mIndices[2]];

        colliderMesh->addTriangle(
            btVector3(a.x, a.y, a.z),
            btVector3(b.x, b.y, b.z),
            btVector3(c.x, c.y, c.z)
        );    
    }
}

void processNode(aiNode *node, const aiScene *scene, btTriangleMesh* colliderMesh) {
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
        processMesh(mesh, scene, colliderMesh);	
    }
    for (int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, colliderMesh);
    }
}

// Takes in absolute path, sets up mesh for non-moving object.
btCollisionShape* Physics::addStaticMap(std::string path) {
    StaticCollider collider;
    collider.mesh = new btTriangleMesh();

    Assimp::Importer importer;
    // TODO: DO i need all this for this.
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // If scene creation failed, flag error.
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error generating scene from model at: " << path << ". Error: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    // Start recursively processing nodes.
    processNode(scene->mRootNode, scene, collider.mesh);

    collider.collision = new btBvhTriangleMeshShape(collider.mesh, true);
    colliders.push_back(collider);

    return colliders.back().collision;
}
