//
//  PatricleSim.cpp
//  OpenGlSimTest
//
//  Created by Jonas Büchner on 24.11.23.
//

#include "ParticleSim.hpp"

void ParticleSim::updateParticles(float deltaTime) {
    std::vector<glm::vec2> adjustmentVector;
    for(int i = 0; i < m_numberOfParticles; ++i) {
        adjustmentVector.push_back(glm::vec2(0,0));}
    for(int i = 0; i < m_numberOfParticles; ++i) {
        
        for (int k = i+1; k < m_numberOfParticles; ++k) {
            if ( glm::length(m_particles[i] - m_particles[k]) < 2 * 5){
                //velocity_k = m_velocities[k];
                // not like this. need to multiply with deltatime
                collideBalls(m_particles[i], m_velocities[i], m_particles[k], m_velocities[k], 5);
                float bounceCost = 0.001;
                glm::vec2 energyLoss = glm::vec2(m_velocities[i].x * bounceCost, m_velocities[i].y * bounceCost);
                m_velocities[i] = m_velocities[i] - energyLoss;
                
            }
        }
        for(int j = 0; j < m_collisionLines.size(); ++j) {
        
            //test distance to line
            float distance = signedDistanceToLineSegment(m_particles[i], m_collisionLines[j].start, m_collisionLines[j].end, 5.f);
        
            //if less than 5, reflect on line
            if (distance < 0.f) {
                collideWithLineSimple(m_particles[i], m_velocities[i], m_collisionLines[j].start, m_collisionLines[j].end);
                //adjustmentVector[i] = glm::vec2(0,0);
                adjustDistanceToLine(m_collisionLines[j].start, m_collisionLines[j].end, m_particles[i], adjustmentVector, i, j);
            };
            
            
        }
    
    }
    for(int i = 0; i < m_numberOfParticles; ++i) {
        m_particles[i] = m_particles[i] + m_velocities[i]* deltaTime;
        //friction
        float friction = 0.0002;
        m_velocities[i] = m_velocities[i] - glm::vec2(m_velocities[i].x *friction, m_velocities[i].y *friction);
        //gravity
        m_velocities[i].y -= 50.f*deltaTime;
    }
    for ( int iteration = 0; iteration < 10; iteration++) {
        for(int i = 0; i < m_numberOfParticles; ++i) {
            
            for (int k = i+1; k < m_numberOfParticles; ++k) {
                separateParticles(adjustmentVector, m_particles[i], m_particles[k], 5, i, k);

            }
        }
    }
    for(int i = 0; i < m_numberOfParticles; ++i) {
        m_particles[i] = m_particles[i] + adjustmentVector[i];
    }
}

void ParticleSim::adjustDistanceToLine(glm::vec2& endpoint1,
                                       glm::vec2& endpoint2,
                                       glm::vec2& particle,
                                       std::vector<glm::vec2>& adjustmentVector,
                                       int particleIndex,
                                       int lineIndex) {
    float distance = signedDistanceToLineSegment(particle + adjustmentVector[particleIndex],
                                                 endpoint1,
                                                 endpoint2,
                                                 5.f);
    if (distance < 0.f) {
        // Calculate the line's direction and normal
        glm::vec2 lineDirection = endpoint2 - endpoint1;
        glm::vec2 lineToParticle = particle - endpoint1;

        // Calculate the cross product in 3D space (z-component only)
        float crossProductZ = lineDirection.x * lineToParticle.y - lineDirection.y * lineToParticle.x;

        // Determine the normal's direction based on the side of the line
        glm::vec2 lineNormal;
        if (crossProductZ > 0) {
            // Particle is on the left side of the line
            lineNormal = glm::vec2(-lineDirection.y, lineDirection.x);
        } else {
            // Particle is on the right side of the line
            lineNormal = glm::vec2(lineDirection.y, -lineDirection.x);
        }
        lineNormal = glm::normalize(lineNormal);

        // Calculate adjustment vector
        glm::vec2 adjustment = (-distance) * lineNormal;
        adjustmentVector[particleIndex] += adjustment; // Accumulate adjustment
    }
}



float ParticleSim::signedDistanceToLineSegment(const glm::vec2 point, const glm::vec2& endpoint1, const glm::vec2& endpoint2, float thickness) {
    glm::vec2 v = endpoint2 - endpoint1;
    glm::vec2 w = point - endpoint1;

    float projection_factor = glm::dot(w, v) / glm::dot(v, v);
    float distance;

    if (projection_factor < 0.0f) {
        // Closest to endpoint1
        distance = glm::length(point - endpoint1);
    } else if (projection_factor > 1.0f) {
        // Closest to endpoint2
        distance = glm::length(point - endpoint2);
    } else {
        // Closest to the line segment itself
        distance = glm::length(w - projection_factor * v);
    }
    return distance - thickness;
}

void ParticleSim::collideBalls(glm::vec2& pos1, glm::vec2& vel1, glm::vec2& pos2, glm::vec2& vel2, float radius) {
    // Calculate the normal of the collision
    glm::vec2 collisionNormal = glm::normalize(pos2 - pos1);

    // Calculate the relative velocity
    glm::vec2 relativeVelocity = vel2 - vel1;

    // Check if particles are moving towards each other
    if (glm::dot(relativeVelocity, collisionNormal) <= 0) {
        // Calculate the velocity components along the collision normal
        float vel1Normal = glm::dot(vel1, collisionNormal);
        float vel2Normal = glm::dot(vel2, collisionNormal);

        // Swap the normal components of the velocities
        glm::vec2 vel1NewNormal = vel2Normal * collisionNormal;
        glm::vec2 vel2NewNormal = vel1Normal * collisionNormal;

        // Calculate the tangential components (unchanged)
        glm::vec2 vel1Tangential = vel1 - vel1Normal * collisionNormal;
        glm::vec2 vel2Tangential = vel2 - vel2Normal * collisionNormal;

        // Recombine to get the new velocities
        vel1 = vel1NewNormal + vel1Tangential;
        vel2 = vel2NewNormal + vel2Tangential;
    }
}


void ParticleSim::collideWithLineSimple(glm::vec2& particlePosition,
                                        glm::vec2& velocity,
                                        glm::vec2& lineStart,
                                        glm::vec2& lineEnd) {
    float endpointRadius = 5;
    // First, check for collision with line segment
    float distanceToLine = signedDistanceToLineSegment(particlePosition, lineStart, lineEnd, 0.0f);
    glm::vec2 lineDelta = lineEnd - lineStart;
    glm::vec2 lineNormal = glm::normalize(glm::vec2(-lineDelta.y, lineDelta.x));

    if (distanceToLine < endpointRadius) {
        // Reflect off the line
        float dotProduct = glm::dot(velocity, lineNormal);
        glm::vec2 reflection = velocity - 2 * dotProduct * lineNormal;
        velocity = reflection;
    } else {
        // Check for collision with line endpoints
        glm::vec2 closestPoint = (glm::length(particlePosition - lineStart) < glm::length(particlePosition - lineEnd)) ? lineStart : lineEnd;
        glm::vec2 toParticle = particlePosition - closestPoint;
        float distanceToEndpoint = glm::length(toParticle);

        if (distanceToEndpoint < endpointRadius) {
            // Reflect off the endpoint
            glm::vec2 endpointNormal = glm::normalize(toParticle);
            float dotProductEndpoint = glm::dot(velocity, endpointNormal);
            glm::vec2 reflectionEndpoint = velocity - 2 * dotProductEndpoint * endpointNormal;
            velocity = reflectionEndpoint;
        }
    }
}


glm::vec2 ParticleSim::collideWithLine(glm::vec2 intersection,
                                       glm::vec2 particlePosition,
                                       glm::vec2 VelocityTimesDeltaTime,
                                       glm::vec2& velocity,
                                       glm::vec2 lineStart,
                                       glm::vec2 lineEnd) {
    float distanceFactor = 1 - (glm::length(particlePosition - intersection) / glm::length(VelocityTimesDeltaTime));
    
    glm::vec2 lineDir = lineEnd - lineStart;
    // Rotate the line direction by 90 degrees to get the normal
    glm::vec2 normal = glm::rotate(lineDir, glm::radians(90.0f));
    normal = glm::normalize(normal); // Normalize the normal
    velocity = glm::reflect(VelocityTimesDeltaTime * distanceFactor, normal);
    
    glm::vec2 newPosition = velocity + intersection;
    return newPosition;
}

void ParticleSim::separateParticles(std::vector<glm::vec2>& adjustmentVector, glm::vec2 &pos1, glm::vec2 &pos2, float radius, int index1, int index2) {
    glm::vec2 pos1cpy = pos1+adjustmentVector[index1];
    glm::vec2 pos2cpy = pos2+adjustmentVector[index2];
    glm::vec2 difference = (pos2cpy) - (pos1cpy);
    float distance = glm::length(difference);

    // Check if the particles are overlapping
    if (distance < 2 * radius) {
        // Calculate the overlap distance
        float overlap = 2 * radius - distance;

        // Normalize the difference vector
        glm::vec2 direction = glm::normalize(difference);

        // Move each particle half of the overlap distance
        glm::vec2 newPos1 = pos1cpy - direction * (overlap / 2.0f -0.005f);
        glm::vec2 newPos2 = pos2cpy + direction * (overlap / 2.0f -0.005f);

        // Update the positions
        adjustmentVector[index1] = newPos1 - pos1;
        adjustmentVector[index2] = newPos2 - pos2;
    }
}

void ParticleSim::createParticles(const int numberOfParticles) {
    m_numberOfParticles = numberOfParticles;
    
    for(int i = 0; i < numberOfParticles; ++i) {
        m_particles.push_back(makeRandomPointOnCircle(300, glm::vec2(0,0)));
        m_velocities.push_back(makeRandomVector(30, 150));
    };
}

void ParticleSim::createBasicColliders() {
    m_collisionLines.push_back(Line(glm::vec2(-319,-239), glm::vec2(-319,239)));
    m_collisionLines.push_back(Line(glm::vec2(-319,-239), glm::vec2(319,-239)));
    m_collisionLines.push_back(Line(glm::vec2(319,239), glm::vec2(319,-239)));
    m_collisionLines.push_back(Line(glm::vec2(319,239), glm::vec2(-319,239)));
    
    //m_collisionLines.push_back(Line(glm::vec2(-200,-200), glm::vec2(-200,200)));
    //m_collisionLines.push_back(Line(glm::vec2(-200,-200), glm::vec2(200,-200)));
    //m_collisionLines.push_back(Line(glm::vec2(200,200), glm::vec2(200,-200)));
    //m_collisionLines.push_back(Line(glm::vec2(200,200), glm::vec2(-200,200)));
    
    m_collisionLines.push_back(Line(glm::vec2(319,239), glm::vec2(8,0)));
    m_collisionLines.push_back(Line(glm::vec2(-319,-239), glm::vec2(-8,0)));
    m_collisionLines.push_back(Line(glm::vec2(319,-239), glm::vec2(8,0)));
    m_collisionLines.push_back(Line(glm::vec2(-319,239), glm::vec2(-8,0)));
    
}

glm::vec2 ParticleSim::makeRandomPointOnCircle(const float diameter, glm::vec2 center) {
    // Random number generation setup
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_real_distribution<> angleDistr(0, 2 * M_PI); // Range for angle
    std::uniform_real_distribution<> radiusDistr(0, diameter / 2.0f); // Range for radius

    // Generate a random angle and a random radius
    float angle = angleDistr(eng);
    float radius = radiusDistr(eng);

    // Calculate the point's coordinates
    float x = center.x + radius * cos(angle);
    float y = center.y + radius * sin(angle);

    return glm::vec2(x, y);
}

glm::vec2 ParticleSim::makeRandomVector(const float minLength, const float maxLength) {
    // Random number generation setup
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_real_distribution<> angleDistr(0, 2 * M_PI); // Range for angle
    std::uniform_real_distribution<> lengthDistr(minLength, maxLength); // Range for length

    // Generate a random angle and a random length
    float angle = angleDistr(eng);
    float length = lengthDistr(eng);

    // Convert polar coordinates to Cartesian coordinates
    float x = length * cos(angle);
    float y = length * sin(angle);

    return glm::vec2(x, y);
}

