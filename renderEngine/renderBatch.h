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

	struct cameraStruct
	{
		glm::vec2 pos;
		glm::vec2 scale;
	};

	class RenderBatch 
	{
		public:
		    GLenum drawType;
		    GLint drawStart;
		    GLint drawCount;

		    ShaderBase shaderBase;

		    size_t bytes_allocated;
		    
		    cameraStruct camera;

		    std::vector<Instance> instances;

		    RenderBatch();
		    RenderBatch(std::vector<unsigned char> &textureData, unsigned width, unsigned height);

		    void setup(ShaderBase inShaderBase);
		    void update();
		    void render();

		private:
			GLuint vao;
			GLuint vbo;

			GLint cameraUniformLocation;
			
			shared_ptr<Texture> texture;
			GLint textureLocation;
	};
}