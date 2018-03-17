#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "VertexArray.h"


struct Particle
{
    float mass;
    float weight;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 netForce;
};

struct Spring
{
    float restLength;
    unsigned int p1, p2;
    float stiffness;
    float dampening;
};