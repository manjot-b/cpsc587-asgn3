#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Spring.h"
#include "Shader.h"
#include "VertexArray.h"

class Engine
{
    public:
        Engine(int argc, const char* argv[]);
        int run();

    private:
        struct DestroyglfwWin
        {
            void operator()(GLFWwindow* ptr)
            {
                glfwDestroyWindow(ptr);
            }
        };

        typedef std::unique_ptr<GLFWwindow, DestroyglfwWin> GLFWwindowPtr;
        GLFWwindowPtr window_;
        bool windowInitialized_;
        // std::vector< std::shared_ptr<VertexArray> > vertexArrays;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexArray> vertexArray;

        uint currentScene = 0;              // starts at index 0
        const uint TOTAL_SCENES = 2;    
        bool rightKeyHeld = false;
        bool leftKeyHeld = false;
        bool rKeyHeld = false;

        std::vector<Spring> springs;
        std::vector<Particle> particles;
        std::vector<float> particlePositions;

        float deltaT = 0.0001;                               // in miliseconds
        uint updatesPerFrame = (1.0f / 60) / deltaT;
        glm::vec3 gravityForce = glm::vec3(0, -9.81, 0);
        float airDampening = 0.0001;

        bool initWindow();
        void initScene();
        void initSingleSpringScene();
        void initMultipleSpringsScene();
        void processInput();
        void update();
        void render();        

        glm::vec3 calcVelocity(const std::vector<glm::vec3>& points, uint time, float deltaT);
        glm::vec3 calcAcceleration(const std::vector<glm::vec3>& points, uint time, float deltaT);
        glm::vec3 calcSpringForce(const Spring& spring);
};