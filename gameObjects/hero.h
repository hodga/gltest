#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "GameObject.h"

namespace hwge 
{
	class Hero : public GameObject
	{
	public:
		Hero() : GameObject(std::shared_ptr<hwgl::Sprite>(nullptr)){};
		Hero(std::shared_ptr<hwgl::Sprite> sprite) : GameObject(sprite) {};

		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		bool checkAndResetContinueFlag();

		virtual void step(World &world) override;

	private:
		glm::vec2 movement;
		bool continueFlag;
	};
}