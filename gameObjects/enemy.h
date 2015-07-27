#pragma once

#include "GameObject.h"
#include <iostream>

namespace hwge 
{
	class Enemy : public GameObject
	{
	public:
		Enemy() : GameObject(std::shared_ptr<hwgl::Sprite>(nullptr)){};
		Enemy(std::shared_ptr<hwgl::Sprite> sprite) : GameObject(sprite) { std::cout << "enemy init!" << std::endl;};
		virtual void step(World &world) override;
	};
}