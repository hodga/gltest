#include "instance.h"

using namespace hwgl;

Instance::Instance(glm::vec2 inTextureCell, glm::vec2 inTextureSize, glm::mat4 nTransform)
{
	textureCell = inTextureCell;
	textureSize = inTextureSize;

	transform = nTransform;
}