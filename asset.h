#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "shaderBase.h"
#include "Texture.h"

namespace hwgl
{
	class Asset
	{
		public:
			std::string fragmentShaderFile;
			std::string vertexShaderFile;
			std::string textureFile;

			GLuint vao;
			GLuint vbo;
			ShaderBase shaderBase;
			shared_ptr<Texture> texture;
			GLint textureLocation;

		    GLenum drawType;
		    GLint drawStart;
		    GLint drawCount;

			//constructor
			Asset();
			Asset(std::string fragmentShaderFile, std::string vertexShaderFile, std::string textureFile);
	};
}
