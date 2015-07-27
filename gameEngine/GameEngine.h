#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include <vector>

#include "../gameObjects/GameObject.h"
#include "../gameObjects/Hero.h"
#include "../renderEngine/renderEngine.h"
#include "World.h"

namespace hwge
{

	class GameEngine
	{
	public:
		void run();

	private:
		hwgl::RenderEngine renderEngine;
		World world;

		void setup();
		void mainLoop();
	};
}