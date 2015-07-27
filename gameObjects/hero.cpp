#include "hero.h"

using namespace hwge;

void Hero::step(World &world) 
{
	if(movement.x != 0 || movement.y != 0)
	{
		move(movement);
		movement = glm::vec2(0,0);
		continueFlag = true;

	}
}

bool Hero::checkAndResetContinueFlag()
{
	bool flag = continueFlag;
	continueFlag = false;
	return flag;
}

void Hero::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	    if (key == GLFW_KEY_W )//&& action == GLFW_PRESS)
	    {
	    	movement = glm::vec2(0,1);
	    }
        if (key == GLFW_KEY_S )//&& action == GLFW_PRESS)
	    {
	    	movement = glm::vec2(0,-1);
	    }
	    if (key == GLFW_KEY_A )//&& action == GLFW_PRESS)
	    {
	    	movement = glm::vec2(-1,0);
	    }
	    if (key == GLFW_KEY_D )//&& action == GLFW_PRESS)
	    {
	    	movement = glm::vec2(1,0);
	    }

}