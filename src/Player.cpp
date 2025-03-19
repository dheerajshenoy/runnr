#include "Player.hpp"

Player::Player(const btVector3 &position, btDiscreteDynamicsWorld *world)
    : world(world)
{

    model = LoadModelFromMesh(GenMeshSphere(1.0f, 100, 100));
    colShape = new btSphereShape(btScalar(1.0f));

    transform.setIdentity();

    btScalar _mass(mass);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.0f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        colShape->calculateLocalInertia(_mass, localInertia);

    transform.setOrigin(position);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, myMotionState, colShape, localInertia);
    body = new btRigidBody(rbInfo);
    body->setRestitution(m_restitution);
    body->setActivationState(DISABLE_DEACTIVATION);

    jumpForce = btVector3(0, 5.0f, 0.0f); // Adjust the force as needed
}

void Player::move(const MoveDirection &dir) noexcept
{
    btVector3 force(0, 0, 0);
    float moveSpeed = 20.0f;

    body->activate(true);

    switch(dir) {
        case MoveDirection::LEFT:
            force += btVector3(-moveSpeed, 0, 0);
            break;

        case MoveDirection::RIGHT:
            force += btVector3(moveSpeed, 0, 0);
            break;
    }

    body->applyCentralForce(force);
    body->setDamping(0.3f, 0.1f);
}

void Player::jump() noexcept
{
    if (isPlayerOnGround())
    {
        body->applyCentralImpulse(jumpForce);
    }
}

bool Player::isOnGround() noexcept
{
    btTransform transform;
    body->getMotionState()->getWorldTransform(transform);
    btVector3 origin = transform.getOrigin();

    float rayLength = 2.0f;
    btVector3 rayEnd = origin + btVector3(0, -rayLength, 0);


    btCollisionWorld::ClosestRayResultCallback rayCallback(origin, rayEnd);
    world->rayTest(origin, rayEnd, rayCallback);

    return rayCallback.hasHit();
}

void Player::render() noexcept
{
    body->getMotionState()->getWorldTransform(transform);
    auto pos = transform.getOrigin();
    DrawModelEx(model,
                (Vector3) { pos.getX(),
                    pos.getY(),
                    pos.getZ() },
                (Vector3) { 0.0f, 0.0f, 1.0f },
                transform.getRotation().getZ(), Vector3One(),
                RED);
}

btTransform Player::position() noexcept
{
    body->getMotionState()->getWorldTransform(transform);
    return transform;
}

bool Player::isPlayerOnGround() noexcept
{
    int numManifolds = world->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++) {
        btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);

        const btCollisionObject* objA = contactManifold->getBody0();
        const btCollisionObject* objB = contactManifold->getBody1();

        if (objA == body || objB == body) {
            for (int j = 0; j < contactManifold->getNumContacts(); j++) {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);

                // Check if the contact is below the player
                if (pt.getPositionWorldOnB().getY() < body->getWorldTransform().getOrigin().getY()) {
                    btVector3 normal = pt.m_normalWorldOnB;

                    // Check if the normal points upward (allowing slight angles)
                    if (normal.dot(btVector3(0, 1, 0)) > 0.5f) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
