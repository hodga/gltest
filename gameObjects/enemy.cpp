#include "enemy.h"
#include <random>

#include <glm/ext.hpp>

#include "../gameEngine/World.h"

using namespace hwge;

float getSign(float number);

void Enemy::step(World &world) 
{
	glm::vec2 heroPos = world.hero.getGridPos();

	glm::vec2 reqMovement = getGridPos() - heroPos;

	if(abs(reqMovement.x) >= abs(reqMovement.y))
	{
		move(glm::vec2(-getSign(reqMovement.x), 0));
	}
	else if(abs(reqMovement.x) < abs(reqMovement.y))
	{
		move(glm::vec2(0, -getSign(reqMovement.y)));
	}
}

float getSign(float number)
{
	if( number == 0) return 1;
	return number/abs(number);
}