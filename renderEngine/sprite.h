#pragma once

#include <string>

#include "instance.h"

namespace hwgl 
{
	class Sprite : public Instance
	{
		friend class RenderEngine;

	public:
		std::string filename;

		glm::vec2 getGridPos();
		void animateMovementToGridPos(glm::vec2 gridPos, double seconds);
		bool isAnimating();

	private:
		glm::vec2 _animationDestination;
		glm::vec2 _animationStep;
		double _animationTotalTime;
		double _animationAcumulatedTime;

		bool animate(double deltaTime); //returns true if sprite changed
		void stopAnimating();

	};
}