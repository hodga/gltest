#include "GameEngine.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <thread>
#include <chrono>
#include <array>
#include <string>

#include "../gameObjects/enemy.h"

using namespace hwge;

Hero *heroptr;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void GameEngine::run()
{
	setup();
	mainLoop();
}

void GameEngine::setup()
{
	world.generate(renderEngine);

	world.hero = Hero(renderEngine.makeSprite("resources/beard.png", "z"));

	heroptr = &world.hero;
	std::unique_ptr<GameObject> uPtr(new Enemy(renderEngine.makeSprite("resources/butters.png", "z")));
	uPtr->move(glm::vec2(6,4));
	world.addGameObject(std::move(uPtr));

	glfwSetKeyCallback(renderEngine.window, keyCallback);
}

void GameEngine::mainLoop()
{
	double lastTime = glfwGetTime();

	while(!glfwWindowShouldClose(renderEngine.window))
	{
		double thisTime = glfwGetTime();
        double deltaTime = thisTime - lastTime;
        lastTime = thisTime;

		glfwPollEvents();

		world.hero.step(world);

		if(world.hero.checkAndResetContinueFlag()) 
		{
			for(auto &obj : world.gameObjects)
			{
				obj->step(world);
			}

		}
		
        renderEngine.step((float)deltaTime);

		GLenum error = glGetError();
        if(error != GL_NO_ERROR)
        {
            std::cerr << "OpenGL Error "<< error << ": " << gluErrorString(error) << std::endl;
        }

        if(glfwGetKey(renderEngine.window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(renderEngine.window, GL_TRUE);
        }
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	heroptr->keyCallback(window, key, scancode, action, mods);
}