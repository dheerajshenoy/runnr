#include "Powerup.hpp"
#include "Entity.hpp"

Powerup::Powerup(const Type& type,
                 const btVector3 &pos)
    : m_type(type)
{
    model = LoadModelFromMesh(GenMeshCube(1, 1, 1));
    m_shape = new btBoxShape(btVector3(1, 1, 1));
    transform.setIdentity();
    transform.setOrigin(pos);

    btDefaultMotionState* powerupMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo powerupCI(0.0f, powerupMotionState, m_shape);
    body = new btRigidBody(powerupCI);
    body->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE); // No physical response
}

Powerup::~Powerup()
{
    UnloadModel(model);
}

void Powerup::render() noexcept
{
    auto pos = transform.getOrigin();
    auto rotation = transform.getRotation();
    auto angle = rotation.getAngle();
    auto axis = rotation.getAxis();
    switch(m_type)
    {
        case Type::JumpBoost:
            DrawModelEx(model, { pos.getX(), pos.getY(), pos.getZ() },
                        { axis.getX(), axis.getY(), axis.getZ() },
                        angle, Vector3One(), BLUE);
            break;
    }
}

void Powerup::update(const float &dt) noexcept
{
    body->getMotionState()->getWorldTransform(transform);
    auto pos = transform.getOrigin();
    pos.setZ(pos.getZ() + dt * m_speed);
    transform.setOrigin(pos);
    body->getMotionState()->setWorldTransform(transform);
    body->setWorldTransform(transform);
}

bool Powerup::isOffScreen(const float &zOffset) noexcept
{
    return transform.getOrigin().getZ() -
        static_cast<btBoxShape*>(m_shape)->getHalfExtentsWithMargin().getZ() - zOffset > 0.0f;
}
