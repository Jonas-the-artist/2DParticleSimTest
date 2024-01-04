//
//  main.cpp
//  OpenGlSimTest
//
//  Created by Jonas Büchner on 13.11.23.
//

// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"
#include "ParticleSim.hpp"
#include "defines.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

std::string readShaderSource(const std::string& shaderFile) {
    std::ifstream shaderStream(shaderFile);
    if (!shaderStream.is_open()) {
        std::cerr << "Could not open file: " << shaderFile << std::endl;
        return "";
    }

    std::stringstream shaderData;
    shaderData << shaderStream.rdbuf();
    shaderStream.close();
    return shaderData.str();
}

GLuint compileShader(const std::string& source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char* shaderSource = source.c_str();
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

struct ShaderProgramData {
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
};


ShaderProgramData createShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    ShaderProgramData result = {0, 0, 0}; // Initialize all IDs to 0

    // Parse shader sources
    std::string vertexShaderSource = readShaderSource(vertexShaderFile);
    std::string fragmentShaderSource = readShaderSource(fragmentShaderFile);

    // Compile shaders
    result.vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    result.fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    if (result.vertexShader == 0 || result.fragmentShader == 0) {
        return result; // Shader compilation failed
    }

    // Create shader program
    result.shaderProgram = glCreateProgram();
    glAttachShader(result.shaderProgram, result.vertexShader);
    glAttachShader(result.shaderProgram, result.fragmentShader);
    glLinkProgram(result.shaderProgram);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(result.shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(result.shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glDeleteProgram(result.shaderProgram);
        result.shaderProgram = 0; // Set shader program to 0 to indicate failure
        return result;
    }

    // At this point, shaders are linked into the program and can be deleted if you don't need them anymore

    return result;
}




int main() {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    // uncomment these lines if on Apple OS X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello OpenGL  ", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    //glfwSetWindowSize(window, 1920, 1080);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "ERROR: " << glewGetErrorString(err) << "\n press enter to end program" << std:: endl;
        std::cin.get();
        return -1;
    }
    
    std::vector<glm::vec2> vertices = {
        // Add your 2D positions here. Each position is a pair (x, y).
        // For example: 
        glm::vec2(0, 0),       // Center of the circle
        glm::vec2(5.0, 0.0),
        glm::vec2(4.924, 0.868),
        glm::vec2(4.698, 1.710),
        glm::vec2(4.330, 2.500),
        glm::vec2(3.830, 3.214),
        glm::vec2(3.214, 3.830),
        glm::vec2(2.500, 4.330),
        glm::vec2(1.710, 4.698),
        glm::vec2(0.868, 4.924),
        glm::vec2(0.0, 5.0),
        glm::vec2(-0.868, 4.924),
        glm::vec2(-1.710, 4.698),
        glm::vec2(-2.500, 4.330),
        glm::vec2(-3.214, 3.830),
        glm::vec2(-3.830, 3.214),
        glm::vec2(-4.330, 2.500),
        glm::vec2(-4.698, 1.710),
        glm::vec2(-4.924, 0.868),
        glm::vec2(-5.0, 0.0),
        glm::vec2(-4.924, -0.868),
        glm::vec2(-4.698, -1.710),
        glm::vec2(-4.330, -2.500),
        glm::vec2(-3.830, -3.214),
        glm::vec2(-3.214, -3.830),
        glm::vec2(-2.500, -4.330),
        glm::vec2(-1.710, -4.698),
        glm::vec2(-0.868, -4.924),
        glm::vec2(0.0, -5.0),
        glm::vec2(0.868, -4.924),
        glm::vec2(1.710, -4.698),
        glm::vec2(2.500, -4.330),
        glm::vec2(3.214, -3.830),
        glm::vec2(3.830, -3.214),
        glm::vec2(4.330, -2.500),
        glm::vec2(4.698, -1.710),
        glm::vec2(4.924, -0.868),
        glm::vec2(5.0, 0.0)
    };
    
    // makes particles with velocities for simulation
    std::vector<glm::vec2> instances = {};
    std::vector<glm::vec2> velocities = {};
    std::vector<Line> collisionLines;
    ParticleSim Sim(instances, velocities, collisionLines);
    Sim.createParticles(100);
    Sim.createBasicColliders();
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // make shader Programm
    // make buffers
    
    GLuint vaoCircles, vaoLines, vboCircle, vboInstances, vboLines;
    
    glGenVertexArrays(1, &vaoCircles);
    glGenVertexArrays(1, &vaoLines);
    glGenBuffers(1, &vboCircle);
    glGenBuffers(1, &vboInstances);
    glGenBuffers(1, &vboLines);
    
    glBindVertexArray(vaoCircles);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboCircle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboInstances);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* instances.size(), instances.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(1);
    
    
    glBindVertexArray(vaoLines);

    glBindBuffer(GL_ARRAY_BUFFER, vboLines);
    glBufferData(GL_ARRAY_BUFFER, 2 * collisionLines.size() * sizeof(glm::vec2), collisionLines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    
    glBindVertexArray(0);
    
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    
    
    ShaderProgramData shaderProgramCircles = createShaderProgram("OpenGlSimTest/vertex_shader_circles.glsl", "OpenGlSimTest/fragment_shader_circles.glsl");
    if (shaderProgramCircles.shaderProgram == 0) {
        std::cout << "ERROR: no Shader for circles \n press enter to end program" << std:: endl;
        std::cin.get();
        return -1;
    }
    
    ShaderProgramData shaderProgramLines = createShaderProgram("OpenGlSimTest/vertex_shader_Lines.glsl", "OpenGlSimTest/fragment_shader_Lines.glsl");
    if (shaderProgramLines.shaderProgram == 0) {
        std::cout << "ERROR: no Shader for lines \n press enter to end program" << std:: endl;
        std::cin.get();
        return -1;
    }
    
    
    // Timing variables
    using Clock = std::chrono::high_resolution_clock;
    auto lastFrameTime = Clock::now();
    float deltaTime = 0.0f;
    
    int framecount = 0;
    float accumulateTime = 0.f;

    while (!glfwWindowShouldClose(window)) {
        auto currentFrameTime = Clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;
        
        
        if (framecount < 10) {
            accumulateTime += deltaTime;
            framecount++;
        }
        if ( framecount == 10) {
            std::ostringstream titleStream;
            titleStream << "Hello OpenGL - Frame Time: " << (int)(10.f/accumulateTime) << " fps";
            std::string title = titleStream.str();
        
            glfwSetWindowTitle(window, title.c_str());
            
            framecount = 0;
            accumulateTime = 0.f;
        }
        
        // Simulation here
        Sim.updateParticles(deltaTime);

        // Bind the instance buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboInstances);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * instances.size(), instances.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render here...
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgramCircles.shaderProgram);

        //draw circles
        glBindVertexArray(vaoCircles);
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, (int)vertices.size(), (int)instances.size());
        glBindVertexArray(0);
        
        
        glBindVertexArray(vaoLines);
        glUseProgram(shaderProgramLines.shaderProgram);
        //draw lines
        glDrawArrays(GL_LINES, 0, 2 * (int)collisionLines.size()); // Each line has two points
        glBindVertexArray(0);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
    //vaoCircles, vaoLines, vboCircle, vboInstances, vboLines
    
    glDeleteVertexArrays(1, &vaoLines);
    glDeleteVertexArrays(1, &vaoCircles);
    
    glDeleteBuffers(1, &vboCircle);
    glDeleteBuffers(1, &vboInstances);
    glDeleteBuffers(1, &vboLines);
    
    // Detach and Delete Shaders
    GLuint vertexShaderID1 = shaderProgramLines.vertexShader; // The ID of your vertex shader
    GLuint fragmentShaderID1 = shaderProgramLines.fragmentShader; // The ID of your fragment shader
    GLuint shaderProgramID1 = shaderProgramLines.shaderProgram; // The ID of your shader program
    
    GLuint vertexShaderID2 = shaderProgramLines.vertexShader; // The ID of your vertex shader
    GLuint fragmentShaderID2 = shaderProgramLines.fragmentShader; // The ID of your fragment shader
    GLuint shaderProgramID2 = shaderProgramCircles.shaderProgram;

    glDetachShader(shaderProgramID1, vertexShaderID1);
    glDetachShader(shaderProgramID1, fragmentShaderID1);
    
    glDetachShader(shaderProgramID2, vertexShaderID2);
    glDetachShader(shaderProgramID2, fragmentShaderID2);

    glDeleteShader(vertexShaderID1);
    glDeleteShader(fragmentShaderID1);
    
    glDeleteShader(vertexShaderID2);
    glDeleteShader(fragmentShaderID2);

    // Delete Shader Program
    glDeleteProgram(shaderProgramID1);
    glDeleteProgram(shaderProgramID2);

    glfwTerminate();
    return 0;
}
