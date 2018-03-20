#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Spring.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

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

        enum Scene 
        {
            SingleSpring,
            MultipleSprings,
            Jello
        };

        typedef std::unique_ptr<GLFWwindow, DestroyglfwWin> GLFWwindowPtr;
        GLFWwindowPtr window_;
        bool windowInitialized_;
        // std::vector< std::shared_ptr<VertexArray> > vertexArrays;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexArray> vertexArray;
        Camera camera;

        uint currentScene = 0;              // starts at index 0
        const uint TOTAL_SCENES = 3;    
        bool rightKeyHeld = false;
        bool leftKeyHeld = false;
        bool rKeyHeld = false;

	    std::vector<uint> indicies;        
        std::vector<Spring> springs;
        std::vector<Particle> particles;
        std::vector<float> particlePositions;

        std::vector<float> groundVertices;
        std::vector<uint> groundIndices;
        std::shared_ptr<VertexArray> groundVertexArray;

        const float EPSILON = 1E-5;
        float deltaT = 0.0001;                               // in miliseconds
        uint updatesPerFrame = (1.0f / 60) / deltaT;
        glm::vec3 gravityForce = glm::vec3(0, -9.81f, 0);
        float airDampening = 0.000001;

        bool initWindow();
        void initScene();
        void initSingleSpringScene();
        void initMultipleSpringsScene();
        void initJelloScene();
        void processInput();
        void update();
        void render();        

        glm::vec3 calcSpringForce(const Spring& spring);
};