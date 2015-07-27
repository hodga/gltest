#include "World.h"

#include <glm/glm.hpp>

using namespace hwge;

void World::generate(hwgl::RenderEngine &renderEngine)
{
	std::string wall("resources/wall1.png");
	std::string lWall("resources/wall2.png");
	std::string grass("resources/ground1.png");
	std::string bGrass("resources/ground2.png");

	//add ground
	for(int i = -10; i < 10; i++)
	{
		for(int j = -2; j < 3; j++)
		{
			auto tile = renderEngine.makeSprite(grass, "_tiles");
			tile->size = glm::vec2(200, 200);
			tile->pos.x = i * 200;

			tile->pos.y = j*-200;
			tiles.push_back(tile);
		}
		
	}

	//add ground
	for(int i = -10; i < 10; i++)
	{
		for(int j = -10; j < -2; j++)
		{
			auto tile = renderEngine.makeSprite(bGrass, "_tiles");
			tile->size = glm::vec2(200, 200);
			tile->pos.x = i * 200;

			tile->pos.y = j*-200;
			tiles.push_back(tile);
		}
		
	}

	//add ground
	for(int i = -10; i < 10; i++)
	{
		for(int j = 3; j < 10; j++)
		{
			auto tile = renderEngine.makeSprite(bGrass, "_tiles");
			tile->size = glm::vec2(200, 200);
			tile->pos.x = i * 200;

			tile->pos.y = j*-200;
			tiles.push_back(tile);
		}
		
	}

	//add wall
	for(int i = -10; i < 10; i++)
	{
		auto tile = renderEngine.makeSprite(wall, "_tiles");
		tile->size = glm::vec2(200, 200);
		tile->pos.x = i * 200;

		tile->pos.y = -3 * -200;
		tiles.push_back(tile);
	}

	//add wall
	for(int i = -10; i < 10; i++)
	{
		auto tile = renderEngine.makeSprite(lWall, "_tiles");
		tile->size = glm::vec2(200, 200);
		tile->pos.x = i * 200;

		tile->pos.y = 2 * -200;
		tiles.push_back(tile);
	}
}

void World::addGameObject(std::unique_ptr<GameObject> gameObject)
{
	gameObjects.push_back(std::move(gameObject));
}