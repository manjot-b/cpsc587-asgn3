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
	//initSingleSpringScene();
	// initMultipleSpringsScene();
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

void Engine::initSingleSpringScene()
{
	struct Particle staticParticle;
	staticParticle.mass = 0;			// makes this a static particle
	staticParticle.position = glm::vec3(0,0.9f,0);
	staticParticle.velocity = glm::vec3(0,0.0,0);
	staticParticle.netForce = glm::vec3(0,0,0);

	struct Particle dynamicParticle;
	dynamicParticle.mass = 0.01;
	dynamicParticle.weight = 1 / dynamicParticle.mass;
	dynamicParticle.position = glm::vec3(0, -0.99f, 0);
	dynamicParticle.velocity = glm::vec3(0,0,0);
	dynamicParticle.netForce = glm::vec3(0,0,0);

	struct Spring spring;
	spring.restLength = 0.5;
	spring.p1 = 0;
	spring.p2 = 1;
	spring.stiffness = 0.5;
	spring.dampening = 1.0 * 2 * sqrt(dynamicParticle.mass * spring.stiffness);

	particles.clear();		// clear previous incase we are resetting scene
	particles.push_back(staticParticle);
	particles.push_back(dynamicParticle);
	
	springs.clear();		// clear previous
	springs.push_back(spring);


	shader = make_shared<Shader>("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader->link();

	particlePositions.clear();
	for (const auto& particle : particles)
	{
		for (uint i = 0; i < 3; i++)
			particlePositions.push_back(particle.position[i]);
	}

	int componentsPerAttrib = 3;	
	vertexArray = make_shared<VertexArray>(
		&componentsPerAttrib, 1, particlePositions.data(), particlePositions.size(), GL_DYNAMIC_DRAW);

	glm::mat4 identity(1.0f);
	camera = Camera(identity, identity);

	shader->use();
	shader->setUniformMatrix4fv("projectionView", camera.getProjectionViewMatrix());
	shader->unuse();
}

void Engine::initMultipleSpringsScene()
{
	struct Particle staticParticle;
	staticParticle.mass = 0;			// makes this a static particle
	staticParticle.position = glm::vec3(0, 0.9f, 0);
	staticParticle.velocity = glm::vec3(0,0.0,0);
	staticParticle.netForce = glm::vec3(0,0,0);

	springs.clear();		// clear previous
	particles.clear();		// clear previous incase we are resetting scene
	particles.push_back(staticParticle);

	uint dynamicParticlesCount = 10;
	for (uint i = 0; i < dynamicParticlesCount; i++)
	{
		struct Particle dynamicParticle;
		dynamicParticle.mass = 0.001;
		dynamicParticle.weight = 1 / dynamicParticle.mass;

		float xPos = particles[i].position.x + 0.05f;			// move to the right of previous particle
		float yPos = staticParticle.position.y + 0.001f * i;		// move up a litte bit
		dynamicParticle.position = glm::vec3(xPos, yPos, 0);
		dynamicParticle.velocity = glm::vec3(0,0,0);
		dynamicParticle.netForce = glm::vec3(0,0,0);

		struct Spring spring;
		spring.restLength = 0.07;
		spring.p1 = i;		// previous
		spring.p2 = i+1;	// current
		spring.stiffness = 0.8;
		spring.dampening = 1.0 * 2 * sqrt(dynamicParticle.mass * spring.stiffness);

		particles.push_back(dynamicParticle);
		springs.push_back(spring);
	}

	shader = make_shared<Shader>("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader->link();

	particlePositions.clear();
	for (const auto& particle : particles)
	{
		for (uint i = 0; i < 3; i++)
			particlePositions.push_back(particle.position[i]);
	}

	int componentsPerAttrib = 3;	
	vertexArray = make_shared<VertexArray>(
		&componentsPerAttrib, 1, particlePositions.data(), particlePositions.size(), GL_DYNAMIC_DRAW);

	glm::mat4 identity(1.0f);
	camera = Camera(identity, identity);

	shader->use();
	shader->setUniformMatrix4fv("projectionView", camera.getProjectionViewMatrix());
	shader->unuse();
}

void Engine::initJelloScene()
{
	// struct Particle staticParticle;
	// staticParticle.mass = 0;			// makes this a static particle
	// staticParticle.position = glm::vec3(0,0.9f,0);
	// staticParticle.velocity = glm::vec3(0,0.0,0);
	// staticParticle.netForce = glm::vec3(0,0,0);

	springs.clear();		// clear previous
	particles.clear();		// clear previous incase we are resetting scene
	// particles.push_back(staticParticle);

	struct Particle dynamicParticle;
	dynamicParticle.velocity = glm::vec3(0,0,0);
	dynamicParticle.netForce = glm::vec3(0,0,0);
	
	uint cubeSize = 4;
	float cubeLength = 5;
	const float smallCubeLength = cubeLength / (cubeSize-1);
	for (uint i = 0; i < cubeSize; i++)	// width
	{
		for (uint j = 0; j < cubeSize; j++)	// height
		{
			for (uint k = 0; k < cubeSize; k++)	// depth
			{
				// if (i == 0 && j == 0 && k == 0 )	// static particle
				// {
				// 	dynamicParticle.mass = 0;
				// 	dynamicParticle.position = glm::vec3(0, 0, -3);
				// }
				// else
				{
					float xPos = smallCubeLength * i - cubeLength/2.f; 
					float yPos = smallCubeLength * j + 5; 
					float zPos = -smallCubeLength * k; 
					dynamicParticle.position = glm::vec3(xPos, yPos, zPos);		
					dynamicParticle.mass = 0.001;
					dynamicParticle.weight = 1 / dynamicParticle.mass;
				}
				particles.push_back(dynamicParticle);
				
			}
		}
	}

	const float MAX_DIST = sqrt( 3.0*smallCubeLength*smallCubeLength );
	// cout << smallCubeLength << " " << MAX_DIST << endl;
	// cout << glm::distance(particles[0].position, particles[21].position) << endl;
	
	struct Spring spring;
	spring.restLength = 0.3;
	spring.stiffness = 1.0;
	spring.dampening = 1.0 * 2 * sqrt(dynamicParticle.mass * spring.stiffness);

	for (uint i = 0; i < particles.size(); i++)
	{
		for (uint j = i+1; j < particles.size(); j++)
		{
			float dist = glm::distance(particles[i].position, particles[j].position);
			if (dist <= MAX_DIST)
			{
				if (i == 0) cout << i << "\t" << j << endl;
				spring.p1 = i;		
				spring.p2 = j;
				spring.restLength = dist;		
				springs.push_back(spring);

				indicies.push_back(i);
				indicies.push_back(j);
			}
		}
	}

	shader = make_shared<Shader>("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader->link();

	particlePositions.clear();
	for (const auto& particle : particles)
	{
		for (uint i = 0; i < 3; i++)
			particlePositions.push_back(particle.position[i]);
	}

	int componentsPerAttrib = 3;	
	vertexArray = make_shared<VertexArray>(
		&componentsPerAttrib, 1, particlePositions.data(), particlePositions.size(), GL_DYNAMIC_DRAW);
	vertexArray->setElementBuffer(indicies.data(), indicies.size());

	// Create ground
	groundVertices = {
		-10, -1, -10,
		-10, -1, 10,
		10, -1, -10,
		10, -1, 10
		};
	groundIndices = {
		0, 1, 2,
		2, 1, 3
	};
	groundVertexArray = make_shared<VertexArray>(&componentsPerAttrib, 1, groundVertices.data(), groundVertices.size());
	groundVertexArray->setElementBuffer(groundIndices.data(), indicies.size());

	glm::mat4 view = glm::lookAt(
		// glm::vec3(1, 1.5, 4.2),		// position
		glm::vec3(0, 1, 16),
		glm::vec3(0, 0, -1),		// looking
		glm::vec3(0, 1, 0)		// up
	);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / 800, 0.1f, 100.0f);
	camera = Camera(view, projection);

	shader->use();
	shader->setUniformMatrix4fv("projectionView", camera.getProjectionViewMatrix());
	shader->unuse();
	
}

int Engine::run()
{
	if (!windowInitialized_)
		return -1;

	while (!glfwWindowShouldClose(window_.get()))
	{
		processInput();
		update();
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

	if (glfwGetKey(window_.get(), GLFW_KEY_R) == GLFW_PRESS && !rKeyHeld)
	{
		rKeyHeld = true;
		initScene();
	}

	if (glfwGetKey(window_.get(), GLFW_KEY_RIGHT) == GLFW_PRESS && !rightKeyHeld)
	{
		rightKeyHeld = true;
		currentScene = currentScene >= TOTAL_SCENES - 1 ? TOTAL_SCENES - 1 : currentScene + 1;
		initScene();
	}

	if (glfwGetKey(window_.get(), GLFW_KEY_LEFT) == GLFW_PRESS && !leftKeyHeld)
	{
		leftKeyHeld = true;
		currentScene = currentScene <= 0 ? 0 : currentScene - 1;
		initScene();
	}

	if (glfwGetKey(window_.get(), GLFW_KEY_RIGHT) == GLFW_RELEASE)
		rightKeyHeld = false;
	if (glfwGetKey(window_.get(), GLFW_KEY_LEFT) == GLFW_RELEASE)
		leftKeyHeld = false;
	if (glfwGetKey(window_.get(), GLFW_KEY_R) == GLFW_RELEASE)
		rKeyHeld = false;
}

void Engine::update()
{

	for (uint i = 0; i < updatesPerFrame; i++)
	{
		// calc spring force on each particle
		for (auto& spring : springs)
		{
			glm::vec3 springForce = calcSpringForce(spring);
			particles[spring.p1].netForce += springForce;
			particles[spring.p2].netForce -= springForce;
		}

		// calc external forces on each particle then update position
		for (auto& particle : particles)
		{
			particle.netForce += particle.mass * gravityForce;
			particle.netForce += -airDampening * particle.velocity;

			if (particle.mass > 0)		// if not a static particle
			{
				particle.velocity += (particle.netForce * particle.weight * deltaT);
				particle.position += particle.velocity * deltaT;
			}

			particle.netForce = glm::vec3(0, 0, 0);
		}
	}

	for (uint i = 0; i < particles.size(); i++)
	{
		for (uint j = 0; j < 3; j++)
			particlePositions[i*3 + j] = particles[i].position[j];
		
	}
	vertexArray->updateBuffer(particlePositions.data(), particlePositions.size());
	
}

glm::vec3 Engine::calcSpringForce(const Spring& spring)
{
	Particle& p1 = particles[spring.p1];
	Particle& p2 = particles[spring.p2];
	
	glm::vec3 forceDirection = p1.position - p2.position;
	float distance = glm::distance(p1.position, p2.position);

	forceDirection = glm::normalize(forceDirection);
	// incase forceDirection is 0, then return
	if ( abs(forceDirection[0]) >= 0.0 && abs(forceDirection[0]) < EPSILON && 
		abs(forceDirection[1]) >= 0.0 && abs(forceDirection[1]) < EPSILON &&
		abs(forceDirection[2]) >= 0.0 && abs(forceDirection[2]) < EPSILON )
	{
		return glm::vec3(0, 0, 0);
	}

	glm::vec3 hooksForce =  forceDirection * (-spring.stiffness * ( distance - spring.restLength));
	// incase hooksForce is 0, then return
	if ( abs(hooksForce[0]) >= 0.0 && abs(hooksForce[0]) < EPSILON && 
		abs(hooksForce[1]) >= 0.0 && abs(hooksForce[1]) < EPSILON &&
		abs(hooksForce[2]) >= 0.0 && abs(hooksForce[2]) < EPSILON )
	{
		return glm::vec3(0, 0, 0);
	}

	glm::vec3 hooksForceNorm = glm::normalize(hooksForce);
	glm::vec3 dampeningForce = hooksForceNorm * -spring.dampening * ( (p1.velocity - p2.velocity) * hooksForce ); 

	return hooksForce + dampeningForce;
}

void Engine::render()
{
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(8);
	shader->use();

	// for (auto& particle : particles)
	// {
	// 	particle.vao->use();
	// 	shader->setUniformMatrix4fv("model", particle.model);
	// 	glDrawArrays(GL_POINTS, 0, 1);
	// 	particle.vao->unuse();
	// }

	vertexArray->use();

	shader->setUniform4fv("uColor", glm::vec4(1, 1, 1, 1));
	glDrawArrays(GL_POINTS, 0, particles.size());

	if (currentScene == Scene::Jello)
	{
		shader->setUniform4fv("uColor", glm::vec4(1, 0.9, 0, 1));
		glDrawElements(GL_LINES, indicies.size(), GL_UNSIGNED_INT, 0);
		
		shader->setUniform4fv("uColor", glm::vec4(0.03, 1, 0.7, 1));
		groundVertexArray->use();
		glDrawElements(GL_TRIANGLES, groundIndices.size(), GL_UNSIGNED_INT, 0);
	} 
	else
	{
		shader->setUniform4fv("uColor", glm::vec4(1, 0.9, 0, 1));
		glDrawArrays(GL_LINE_STRIP, 0, particles.size());		
	}
	vertexArray->unuse();

	shader->unuse();
	glfwSwapBuffers(window_.get());
	glfwPollEvents();
}

void Engine::initScene()
{
	switch (currentScene)
	{
		case 0 : initSingleSpringScene(); break;
		case 1 : initMultipleSpringsScene(); break;
		case 2 : initJelloScene(); break;
		default : break;
	}
}