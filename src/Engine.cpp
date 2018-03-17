#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <map>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include "Engine.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Spring.h"

using namespace std;

Engine::Engine(int argc, const char *argv[])
{
	if (!initWindow())
	{
		cerr << "Failed to  initilize GLFW" << endl;
		windowInitialized_ = false;
	}
	windowInitialized_ = true;
	initScene();
}

bool Engine::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = GLFWwindowPtr(glfwCreateWindow(800, 800, "Roller Coaster", NULL, NULL));

	if (!window_)
	{
		cout << "Failed to create GLFW window. TERMINATING" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window_.get());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return false;
	}

	glViewport(0, 0, 800, 800);
	glfwSetWindowUserPointer(window_.get(), this);

	glfwSetFramebufferSizeCallback(window_.get(),
								   [](GLFWwindow *window, int width, int height) {
									   glViewport(0, 0, width, height);
								   });
	glEnable(GL_DEPTH_TEST);

	return true; // if we made it here then success
}

void Engine::initScene()
{
	struct Particle staticParticle;
	staticParticle.mass = 0;
	staticParticle.position = glm::vec3(0,0.9f,0);
	staticParticle.velocity = glm::vec3(0,0.0,0);
	staticParticle.netForce = glm::vec3(0,0,0);
	staticParticle.model = glm::mat4(1.0f);

	struct Particle dynamicParticle;
	dynamicParticle.mass = 0.01;
	dynamicParticle.weight = 1 / dynamicParticle.mass;
	dynamicParticle.position = glm::vec3(0, -0.5f, 0);
	dynamicParticle.velocity = glm::vec3(0,0,0);
	dynamicParticle.netForce = glm::vec3(0,0,0);
	dynamicParticle.model = glm::mat4(1.0f);

	struct Spring spring;
	spring.restLength = 0.5;
	spring.p1 = 0;
	spring.p2 = 1;
	spring.stiffness = 0.5;
	spring.dampening = 0.8 * 2 * sqrt(dynamicParticle.mass * spring.stiffness);

	particles.push_back(staticParticle);
	particles.push_back(dynamicParticle);
	springs.push_back(spring);


	shader = make_shared<Shader>("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader->link();
	// vertexArrays.clear();
	int componentsPerAttrib = 3;
	for (auto& particle : particles)
	{
		float * data = glm::value_ptr(particle.position);
		particle.vao = make_shared<VertexArray>(&componentsPerAttrib, 1, data, 3);
	}
	
	// glm::mat4 view = glm::lookAt(
	// 		glm::vec3(0.05f, 0.3f, 0.8f),	// camera position
	// 		glm::vec3(0.05f, 0.2f, 0),		// where camera is lookin
	// 		glm::vec3(0, 1, 0)				// up vector
    //         );
    // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    // Camera camera = Camera(view, projection);

}

int Engine::run()
{
	if (!windowInitialized_)
		return -1;

	float time = 0;
	while (!glfwWindowShouldClose(window_.get()))
	{
		processInput();
		time = update(time);
		render();
	}

	glfwTerminate();
	return 0;
}

void Engine::processInput()
{
	if (glfwGetKey(window_.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window_.get(), true);
	}
}

float Engine::update(float time)
{

	for (uint i = 0; i < updatesPerFrame; i++)
	{
		for (auto& spring : springs)
		{
			glm::vec3 springForce = calcSpringForce(spring);
			particles[spring.p1].netForce += springForce;
			particles[spring.p2].netForce -= springForce;
		}

		for (auto& particle : particles)
		{
			particle.netForce += particle.mass * gravityForce;

			if (particle.mass > 0)
			{
				particle.velocity += (particle.netForce * particle.weight * deltaT);
				glm::vec3 translation = particle.position;
				particle.position += particle.velocity * deltaT;
				translation = particle.position - translation;
				particle.model = glm::translate(particle.model, translation);
			}

			particle.netForce = glm::vec3(0, 0, 0);
		}

		time += deltaT;
	}
	return time;
}

glm::vec3 Engine::calcVelocity(const vector<glm::vec3>& points, uint time, float deltaT)
{
	if (time >= points.size()) time %= points.size();
	return ( points[time + 1] - points[time] ) / deltaT;
}

glm::vec3 Engine::calcAcceleration(const vector<glm::vec3>& points, uint time, float deltaT)
{
	if (time >= points.size()) time %= points.size();
	if (time <= 0) time = 1;
	return ( points[time + 1] - 2.0f*points[time] + points[time - 1] ) / (deltaT * deltaT);
}

glm::vec3 Engine::calcSpringForce(const Spring& spring)
{
	Particle& p1 = particles[spring.p1];
	Particle& p2 = particles[spring.p2];
	
	glm::vec3 forceDirection = p1.position - p2.position;
	float distance = glm::distance(p1.position, p2.position);
	forceDirection = glm::normalize(forceDirection);
	
	glm::vec3 hooksForce =  forceDirection * (-spring.stiffness * ( distance - spring.restLength));
	glm::vec3 hooksForceNorm = glm::normalize(hooksForce);

	glm::vec3 dampeningForce = hooksForceNorm * -spring.dampening * ( (p1.velocity - p2.velocity) * hooksForce ); 

	return hooksForce + dampeningForce;
}

void Engine::render()
{
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(15);
	shader->use();

	for (auto& particle : particles)
	{
		particle.vao->use();
		shader->setUniformMatrix4fv("model", particle.model);
		glDrawArrays(GL_POINTS, 0, 1);
		particle.vao->unuse();
	}

	shader->unuse();
	glfwSwapBuffers(window_.get());
	glfwPollEvents();
}