#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "shaderBase.h"
#include "textureLib/Texture.h"
#include "instance.h"

namespace hwgl 
{
	class RenderBatch 
	{
		public:
			GLuint vao;
			GLuint vbo;
			ShaderBase shaderBase;
			shared_ptr<Texture> texture;
			GLint textureLocation;

		    GLenum drawType;
		    GLint drawStart;
		    GLint drawCount;

		    size_t bytes_allocated;

		    std::vector<Instance> instances;

		    RenderBatch();
		    RenderBatch(std::vector<unsigned char> &textureData, unsigned width, unsigned height);

		    void setup(ShaderBase inShaderBase);
		    void update();
		    void render();
	};
}