#pragma once

#include <glm/glm.hpp>

#include "gameObjectConstants.h"

#include "../renderEngine/sprite.h"

namespace hwge 
{
	class World;
	class GameObject 
	{
	public:
		GameObject(std::shared_ptr<hwgl::Sprite> sprite)
		{
			_sprite = sprite;
			_gridPos = glm::vec2(0,0);
		}
		virtual ~GameObject() {}



		void move(glm::vec2 movement)
		{
			if(!_sprite->isAnimating())
			{
				_gridPos += movement;
				_sprite->animateMovementToGridPos(_gridPos, HWGE_MOVE_ANIMATION_SPEED);
			}
		}

		glm::vec2 getGridPos()
		{
			return _gridPos;
		}

		virtual void step(World &world) = 0;

	private:
		std::shared_ptr<hwgl::Sprite> _sprite;
		glm::vec2 _gridPos;


	};
}