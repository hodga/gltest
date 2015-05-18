#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace hwgl
{
	class Texture
	{
		public:
		GLuint textureHandle;

		Texture(std::string filename, 
				GLint minMagFilter = GL_LINEAR, 
				GLint wrapMode = GL_CLAMP_TO_EDGE);
		Texture(std::vector<unsigned char> &image, 
				 unsigned width, 
				 unsigned height, 
				 GLint minMagFilter = GL_LINEAR, 
				 GLint wrapMode = GL_CLAMP_TO_EDGE);
		~Texture();
	};
}

