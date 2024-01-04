//
//  PatricleSim.hpp
//  OpenGlSimTest
//
//  Created by Jonas Büchner on 24.11.23.
//

#ifndef ParticleSim_hpp
#define ParticleSim_hpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>   // For trigonometric functions and M_PI
#include <random>  // For random number generation

#include "defines.hpp"

#include "glm/glm.hpp"
#include "ParticleSim.hpp"
#include <glm/gtx/rotate_vector.hpp>

class ParticleSim {
public:
    ParticleSim(std::vector<glm::vec2>& particles, std::vector<glm::vec2>& velocities, std::vector<Line>& collisionLines)
        : m_particles(particles), m_velocities(velocities), m_collisionLines(collisionLines) {
        // Constructor body (if needed)
    }
    //setup
    void createParticles(const int numberOfParticles);
    void createBasicColliders();
    
    glm::vec2 makeRandomPointOnCircle(const float diameter, glm::vec2 center);
    glm::vec2 makeRandomVector(const float minLength, const float maxLength);
    
    std::optional<glm::vec2> findIntersection(const glm::vec2& A1, const glm::vec2& A2,
                                              const glm::vec2& B1, const glm::vec2& B2);
    glm::vec2 collideWithLine(glm::vec2 intersection,
                              glm::vec2 particlePosition,
                              glm::vec2 VelocityTimesDeltaTime,
                              glm::vec2& velocity,
                              glm::vec2 lineStart,
                              glm::vec2 lineEnd);
    
    void collideWithLineSimple(glm::vec2& particlePosition,
                                    glm::vec2& velocity,
                                    glm::vec2& lineStart,
                                    glm::vec2& lineEnd);
    
    void adjustDistanceToLine(glm::vec2& endpoint1,
                              glm::vec2& endpoint2,
                              glm::vec2& particle, 
                              std::vector<glm::vec2>& adjustmentVector,
                              int particleIndex,
                              int lineIndex);
    float signedDistanceToLineSegment(const glm::vec2 point, const glm::vec2& endpoint1, const glm::vec2& endpoint2, float thickness);
    void collideBalls(glm::vec2& pos1, glm::vec2& vel1, glm::vec2& pos2, glm::vec2& vel2, float radius);
    void separateParticles(std::vector<glm::vec2>& adjustmentVector, glm::vec2 &pos1, glm::vec2 &pos2, float radius, int index1, int index2);
    
    //update and colision functions
    void updateParticles(float deltaTime);

private:
    std::vector<glm::vec2>& m_particles;
    std::vector<glm::vec2>& m_velocities;
    std::vector<Line>& m_collisionLines;
    int m_numberOfParticles = 0;
};




#endif /* PatricleSim_hpp */
