#pragma once

#include "bullet.hpp"
#include <raylib.h>
#include <iostream>

class Box
{
    public:
    Box()
        {
            model = LoadModel("../box.obj");
            UploadMesh(&model.meshes[0], true); // This creates indices if none exist
            auto mesh = model.meshes[0];

            if (mesh.triangleCount <= 0 || !mesh.indices) {
                std::cerr << "Error: Mesh has no triangles or indices." << std::endl;
                return;
            }

            triangleMesh = new btTriangleMesh();
            for (int i = 0; i < mesh.triangleCount; i++)
            {
                int index0 = mesh.indices[i * 3];
                int index1 = mesh.indices[i * 3 + 1];
                int index2 = mesh.indices[i * 3 + 2];

                // Access vertex positions using the indices
                btVector3 v0(mesh.vertices[index0 * 3], mesh.vertices[index0 * 3 + 1], mesh.vertices[index0 * 3 + 2]);
                btVector3 v1(mesh.vertices[index1 * 3], mesh.vertices[index1 * 3 + 1], mesh.vertices[index1 * 3 + 2]);
                btVector3 v2(mesh.vertices[index2 * 3], mesh.vertices[index2 * 3 + 1], mesh.vertices[index2 * 3 + 2]);

                // Add triangle to the triangle mesh
                triangleMesh->addTriangle(v0, v1, v2);
            }

            meshShape = new btBvhTriangleMeshShape(triangleMesh, true);
            meshShape->setLocalScaling(btVector3(scale, scale, scale));

            transform.setIdentity();
            transform.setOrigin(btVector3(0, 2, -10));

            btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, meshShape);
            body = new btRigidBody(rbInfo);
            // body->setActivationState(DISABLE_DEACTIVATION);

        }
    ~Box()
        {
            UnloadModel(model);
            delete meshShape;
        }

    void render() noexcept
        {
            auto pos = transform.getOrigin();
            DrawModel(model, {
                    pos.getX(),
                    pos.getY(),
                    pos.getZ()
                }, scale, RAYWHITE);
        }

    void update(const float &dt) noexcept
        {
            body->getMotionState()->getWorldTransform(transform);
            auto pos = transform.getOrigin();
            pos.setZ(pos.getZ() + dt * 5.0f);
            transform.setOrigin(pos);
            body->getMotionState()->setWorldTransform(transform);
            body->setWorldTransform(transform);
        }

    btRigidBody *body { nullptr };
    btTransform transform;
    Model model;

    private:
    float scale = 2.0f;

    btTriangleMesh *triangleMesh { nullptr };
    btBvhTriangleMeshShape *meshShape { nullptr };
};
