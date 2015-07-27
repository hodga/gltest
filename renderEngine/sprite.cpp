#include "sprite.h"

#include "rendererConstants.h"


#include <iostream>
using namespace std;

using namespace hwgl;

glm::vec2 Sprite::getGridPos()
{
	float cellSize = HWGL_GRID_CELL_SIZE;
	return pos/cellSize;
}

void Sprite::animateMovementToGridPos(glm::vec2 gridPos, double seconds)
{
	gridPos *= HWGL_GRID_CELL_SIZE;
	
	_animationDestination = gridPos;
	_animationTotalTime = seconds;
	_animationAcumulatedTime = 0;

	_animationStep = (gridPos - pos) / (float)seconds;

	//cout << "really moving to " << _animationDestination.x << " : " << _animationDestination.y << "at speed " << _animationStep.x << " : " << _animationStep.y << endl;
}

bool Sprite::animate(double deltaTime)
{
	if(!isAnimating()) return false;
	_animationAcumulatedTime += deltaTime;
	
	pos += _animationStep * (float)deltaTime;

	//cout << "dest " << abs(pos.x - _animationDestination.x) << " : " << abs(pos.y - _animationDestination.y) << "current step " << _animationStep.x << " : " << _animationStep.y << endl;
	if(_animationTotalTime - _animationAcumulatedTime <= 0)
	{
		pos = _animationDestination;
		stopAnimating();
	}
	
	return true;
}

bool Sprite::isAnimating()
{
	return abs(_animationStep.x) > 0 || abs(_animationStep.y) > 0;
}

void Sprite::stopAnimating()
{
	_animationStep = glm::vec2(0,0);
}