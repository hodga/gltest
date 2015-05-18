#include "renderBatch.h"

#include <iostream>

using namespace hwgl;
using namespace std;

RenderBatch::RenderBatch() {}

RenderBatch::RenderBatch(std::vector<unsigned char> &textureData, unsigned width, unsigned height) 
{
	bytes_allocated = 0;
	
	texture.reset( new Texture(textureData, width, height));
}

void RenderBatch::setup(ShaderBase inShaderBase) 
{
	shaderBase = inShaderBase;

	glUseProgram(shaderBase.programHandle);
	GLint cameraUniformLocation = glGetUniformLocation(shaderBase.programHandle, "cameraPostion");
	glUniformMatrix4fv(cameraUniformLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

	// make and bind the VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // make and bind the VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //input to shader
  	glEnableVertexAttribArray(shaderBase.attrib("textureCellCoord"));
  	glEnableVertexAttribArray(shaderBase.attrib("size"));

  	glVertexAttribPointer(shaderBase.attrib("textureCellCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) (sizeof(float)*0));
  	glVertexAttribPointer(shaderBase.attrib("size"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) (sizeof(float)*2));

  	glVertexAttribDivisor(shaderBase.attrib("textureCellCoord"), 1);
  	glVertexAttribDivisor(shaderBase.attrib("size"), 1);

  	//bind mat4

	int pos = shaderBase.attrib("transform");
	int pos1 = pos + 0; 
	int pos2 = pos + 1; 
	int pos3 = pos + 2; 
	int pos4 = pos + 3; 

	glEnableVertexAttribArray(pos1);
	glEnableVertexAttribArray(pos2);
	glEnableVertexAttribArray(pos3);
	glEnableVertexAttribArray(pos4);

	glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) (sizeof(float) * 4));
	glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*)(sizeof(float) * 8));
	glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*)(sizeof(float) * 12));
	glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*)(sizeof(float) * 16));

	glVertexAttribDivisor(pos1, 1);
	glVertexAttribDivisor(pos2, 1);
	glVertexAttribDivisor(pos3, 1);
	glVertexAttribDivisor(pos4, 1);

  	//----

  	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  	glUseProgram(0);

  	textureLocation = glGetUniformLocation(shaderBase.programHandle, "tex");
}

void RenderBatch::update() 
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	size_t bytes_needed = sizeof(Instance) * instances.size();
	if(bytes_needed > bytes_allocated) 
	{
		glBufferData(GL_ARRAY_BUFFER, bytes_needed, &(instances[0].textureCell), GL_STREAM_DRAW);
		bytes_allocated = bytes_needed;
	}
	else 
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytes_needed, &(instances[0].textureCell));
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderBatch::render() 
{
	glUniform1i(textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->textureHandle);
	glBindVertexArray(vao);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances.size());

	//unbind everything
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}