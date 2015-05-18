#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

namespace hwgl 
{
	class Instance
	{
		public:
			//glm::mat4 transform;
			//std::array<glm::vec2, 4> textureCoordinate;
			//std::array<float, 6> data;
			glm::vec2 textureCell;
			glm::vec2 textureSize;
			glm::mat4 transform;

			Instance(glm::vec2 inTextureCell, glm::vec2 inTextureSize, glm::mat4 nTransform);
	};
}