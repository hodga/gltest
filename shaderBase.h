#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <memory>
#include <vector>
using namespace std;

namespace hwgl
{
	
	class ShaderBase
        {
		public:
			vector<GLuint> shaderHandles;
			GLuint programHandle;

			GLuint addShaderFromSource(const string &source, GLenum shaderType);
			GLuint addShaderFromFile(const string &fileName, GLenum shaderType);
			GLuint makeProgram();

			ShaderBase();
			~ShaderBase();

			GLint attrib(const GLchar* attribName) const;
			GLint uniform(const GLchar* uniformName) const;
	};
}
