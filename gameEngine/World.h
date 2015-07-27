#pragma once

#include <vector>
#include "../renderEngine/sprite.h"
#include "../renderEngine/renderEngine.h"
#include "../gameObjects/GameObject.h"
#include "../gameObjects/Hero.h"

namespace hwge
{
	class World
	{
	public:
		friend class GameEngine;
		Hero hero;

		void generate(hwgl::RenderEngine &renderEngine);

		void addGameObject(std::unique_ptr<GameObject> gameObject);
	private:
		std::vector<std::shared_ptr<hwgl::Sprite> > tiles;

		vector<std::unique_ptr<GameObject> > gameObjects;
	};
}