#include "Platform.hpp"
#include <raylib.h>

#define RAD2DEG 57.2958

Platform::Platform(const btVector3 &position,
                   const btVector3 &dimension,
                   const float &speed)
    : m_speed(speed)
{
    model = LoadModelFromMesh(GenMeshCube(dimension.getX(), dimension.getY(), dimension.getZ()));
    colShape = new btBoxShape(dimension * 0.5);

    transform.setIdentity();
    transform.setOrigin(position);

    btScalar mass(0.f);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass, localInertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    body = new btRigidBody(rbInfo);
    body->setRestitution(m_restitution);
    body->setActivationState(DISABLE_DEACTIVATION);
}

void Platform::cleanup(btDiscreteDynamicsWorld *world) noexcept
{
    if (body)
    {
        if (body->getMotionState())
        {
            delete body->getMotionState();
        }

        world->removeRigidBody(body);
        delete body;
        body = nullptr;
    }

    if (colShape)
    {
        delete colShape;
        colShape = nullptr;
    }

    UnloadModel(model);
}

void Platform::update(const float &dt) noexcept
{
    body->getMotionState()->getWorldTransform(transform);
    auto pos = transform.getOrigin();
    pos.setZ(pos.getZ() + dt * m_speed);
    transform.setOrigin(pos);
    body->getMotionState()->setWorldTransform(transform);
    body->setWorldTransform(transform);
}

void Platform::render() noexcept
{

    body->getMotionState()->getWorldTransform(transform);
    auto pos = transform.getOrigin();
    auto rotation = transform.getRotation();
    Vector3 axis = { rotation.getAxis().getX(),
        rotation.getAxis().getY(),
        rotation.getAxis().getZ() };
    float angle = rotation.getAngle() * RAD2DEG;
    DrawModelEx(model,
                (Vector3) { pos.getX(), pos.getY(), pos.getZ()},
                axis,
                angle, Vector3One(), RED);
}

bool Platform::isOffScreen(const float &zOffset) noexcept
{
    return transform.getOrigin().getZ() -
        static_cast<btBoxShape*>(colShape)->getHalfExtentsWithMargin().getZ() - zOffset > 0.0f;
}


void Platform::setRotation(const btVector3 &axis,
                           const btScalar &angle) noexcept
{
    btQuaternion rotation;
    rotation.setRotation(axis, angle);
    transform.setRotation(rotation);

    if (body)
    {
        body->setWorldTransform(transform);
        body->getMotionState()->setWorldTransform(transform);
    }
}
