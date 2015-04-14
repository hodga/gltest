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
			std::array<float, 6> data;

			Instance(glm::vec2 pos, glm::vec2 inTextureCell, glm::vec2 inTextureSize);
	};
}