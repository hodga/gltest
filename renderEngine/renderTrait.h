#pragma once

#include <string>
#include <glm/glm.hpp>

namespace hwgl 
{
	class RenderTrait
	{
	public:
		virtual ~RenderTrait() {}
		RenderTrait() {}

		virtual std::string getImagePath() = 0;
		virtual std::string getBatchName() 
		{
			return "-";
		}
		virtual void setTransformPointer(glm::mat4 *nTransform) //fuck you shared ptr, ugly but is there any faster way when you have full control?
		{
			transform = nTransform;
		}
		glm::mat4 *transform;
	//private:
		
	};
}