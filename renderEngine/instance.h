#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hwgl 
{
	class Instance
	{
		friend class RenderEngine;
		public:
			
			glm::vec2 pos;
			glm::vec2 size;

			//Instance();
			//Instance(glm::vec2 inPos, glm::vec2 inSize, glm::vec2 inTextureCell, glm::vec2 inTextureSize);

		private:
			glm::vec2 textureCell;
			glm::vec2 textureSize;

			//glm::mat4 transform;
	};
}