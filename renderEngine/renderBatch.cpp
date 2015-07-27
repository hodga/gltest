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

	camera.scale = glm::vec2(0.001, 0.001);
	glUseProgram(shaderBase.programHandle);
	cameraUniformLocation = glGetUniformLocation(shaderBase.programHandle, "cameraPostion");
	
	glUniformMatrix4fv(cameraUniformLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::scale(glm::mat4(), glm::vec3(camera.scale.x,camera.scale.y, 1)), glm::vec3(camera.pos.x,camera.pos.y,0)))); //camera focus

	// make and bind the VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // make and bind the VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //input to shader
  	glEnableVertexAttribArray(shaderBase.attrib("pos"));
  	glVertexAttribPointer(shaderBase.attrib("pos"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) (sizeof(float)*0));
  	glVertexAttribDivisor(shaderBase.attrib("pos"), 1);

  	glEnableVertexAttribArray(shaderBase.attrib("size"));
  	glVertexAttribPointer(shaderBase.attrib("size"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) (sizeof(float)*2));
  	glVertexAttribDivisor(shaderBase.attrib("size"), 1);

  	glEnableVertexAttribArray(shaderBase.attrib("textureCellCoord"));
  	glVertexAttribPointer(shaderBase.attrib("textureCellCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) (sizeof(float)*4));
  	glVertexAttribDivisor(shaderBase.attrib("textureCellCoord"), 1);

  	glEnableVertexAttribArray(shaderBase.attrib("textureCellSize"));
  	glVertexAttribPointer(shaderBase.attrib("textureCellSize"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) (sizeof(float)*6));
  	glVertexAttribDivisor(shaderBase.attrib("textureCellSize"), 1);

  	/*bind mat4

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

  	*///----

  	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  	glUseProgram(0);

  	textureLocation = glGetUniformLocation(shaderBase.programHandle, "tex");
}

void RenderBatch::update() 
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Instance) * instances.size(), &(instances[0].pos), GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderBatch::render() 
{
	glUniformMatrix4fv(cameraUniformLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::scale(glm::mat4(), glm::vec3(camera.scale.x,camera.scale.y, 1)), glm::vec3(camera.pos.x,camera.pos.y,0)))); //camera focus
	glUniform1i(textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->textureHandle);
	glBindVertexArray(vao);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances.size());

	//unbind everything
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}