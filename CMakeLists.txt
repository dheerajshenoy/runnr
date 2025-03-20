cmake_minimum_required(VERSION 3.20)
project(Runnr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall -Wextra")


find_package(Bullet REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/runnr.cpp
    src/GameScene.cpp
    src/Player.cpp
    src/Platform.cpp
    src/Powerup.cpp
    src/PlayerCollisionCallback.cpp
    src/Entity.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${BULLET_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    raylib
    BulletDynamics
    BulletCollision
    LinearMath)
