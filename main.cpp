#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <thread>
#include <chrono>
#include <array>


#include "asset.h"
#include "instance.h"

using namespace hwgl;

// globals
GLFWwindow* gWindow = NULL;

Asset gAsset;

std::vector<Instance> gInstances;

Instance *instance;

glm::mat4 gCamera;

size_t bytes_allocated;

void onError(int errorCode, const char *msg)
{
	throw std::runtime_error(msg);
}

void loadTriangle() 
{
	// make and bind the VAO
    glGenVertexArrays(1, &gAsset.vao);
    glBindVertexArray(gAsset.vao);
    
    // make and bind the VBO
    glGenBuffers(1, &gAsset.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gAsset.vbo);
    
    // Put the three triangle vertices (XYZ) and texture coordinates (UV) into the VBO
    GLfloat vertexData[] = {
        //  X     Y     Z       U     V
         0.0f, 0.8f, 0.0f,   0.5f, 0.0f,
        -0.8f,-0.8f, 0.0f,   1.0f, 1.0f,
         0.8f,-0.8f, 0.0f,   0.0f, 1.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gAsset.shaderBase.attrib("vert"));
    glVertexAttribPointer(gAsset.shaderBase.attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
        
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(gAsset.shaderBase.attrib("vertTexCoord"));
    glVertexAttribPointer(gAsset.shaderBase.attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void setup()
{
	glUseProgram(gAsset.shaderBase.programHandle);
	GLint cameraUniformLocation = glGetUniformLocation(gAsset.shaderBase.programHandle, "cameraPostion");
	glUniformMatrix4fv(cameraUniformLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

	// make and bind the VAO
    glGenVertexArrays(1, &gAsset.vao);
    glBindVertexArray(gAsset.vao);
    
    // make and bind the VBO
    glGenBuffers(1, &gAsset.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gAsset.vbo);

    //input to shader
    glEnableVertexAttribArray(gAsset.shaderBase.attrib("pos")); 
  	glEnableVertexAttribArray(gAsset.shaderBase.attrib("textureCellCoord"));
  	glEnableVertexAttribArray(gAsset.shaderBase.attrib("size"));

  	glVertexAttribPointer(gAsset.shaderBase.attrib("pos"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) 0);
  	glVertexAttribPointer(gAsset.shaderBase.attrib("textureCellCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) (sizeof(float)*2));
  	glVertexAttribPointer(gAsset.shaderBase.attrib("size"), 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (GLvoid*) ((sizeof(float)*2)+(sizeof(float)*2)));

  	glVertexAttribDivisor(gAsset.shaderBase.attrib("pos"), 1);
  	glVertexAttribDivisor(gAsset.shaderBase.attrib("textureCellCoord"), 1);
  	glVertexAttribDivisor(gAsset.shaderBase.attrib("size"), 1);

  	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  	glUseProgram(0);
}

void loadInstances()
{
	glm::vec2 texCellCoord = glm::vec2(0,0);
	glm::vec2 texSize = glm::vec2(1,1);

	Instance a(glm::vec2(0,0), texCellCoord, texSize);
	gInstances.push_back(a);

	Instance b(glm::vec2(0.5,0), texCellCoord, texSize);
	gInstances.push_back(b);

	//glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(0.5,0,0)) * glm::scale(glm::mat4(), glm::vec3(0.1,0.1,1));
	Instance c(glm::vec2(0.3,0.3), texCellCoord, texSize);
	gInstances.push_back(c);
	instance = &(gInstances.back());
}

void Render()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(gAsset.shaderBase.programHandle);

	glUniform1i(gAsset.textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gAsset.texture->textureHandle);
	glBindVertexArray(gAsset.vao);

	//glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0, gInstances.size());
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, gInstances.size());

	/*for(Instance instance : gInstances)
	{
		instance.Render();
	}*/

	//unbind everything
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	glfwSwapBuffers(gWindow);
}

static void Update(float secondsElapsed) {

    //move position of camera based on WASD keys, and XZ keys for up and down
   /* const float moveSpeed = 2; //units per second
    if(glfwGetKey(gWindow, 'S'))
    {
        instance->transform *= glm::translate(glm::mat4(), glm::vec3(0,-moveSpeed*secondsElapsed,0));
    } 
    else if(glfwGetKey(gWindow, 'W'))
    {
        instance->transform *= glm::translate(glm::mat4(), glm::vec3(0,moveSpeed*secondsElapsed,0));
    }
    if(glfwGetKey(gWindow, 'A'))
    {
        instance->transform *= glm::translate(glm::mat4(), glm::vec3(-moveSpeed*secondsElapsed,0,0));
    } 
    else if(glfwGetKey(gWindow, 'D'))
    {
        instance->transform *= glm::translate(glm::mat4(), glm::vec3(moveSpeed*secondsElapsed,0,0));
    }*/


	glBindBuffer(GL_ARRAY_BUFFER, gAsset.vbo);
 
	size_t bytes_needed = sizeof(Instance) * gInstances.size();
	if(bytes_needed > bytes_allocated) 
	{
		glBufferData(GL_ARRAY_BUFFER, bytes_needed, &(gInstances[0].data[0]), GL_STREAM_DRAW);
		bytes_allocated = bytes_needed;
	}
	else 
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytes_needed, &(gInstances[0].data[0]));
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int main(int argc, char*argv[])
{
	glfwSetErrorCallback(onError);
	if(!glfwInit())
	{
		std::cout << "Error initializing glfw";
		return 0;
	}

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	gWindow = glfwCreateWindow((int)800, (int)600, "OpenGL Test", NULL, NULL);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if(!gWindow) 
	{
		throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
	}
	
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
	}

	// print out some info about the graphics drivers
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	
	if(!GLEW_VERSION_3_2)
	{
		throw std::runtime_error("OpenGL 3.2 API is not available");
	}

	gAsset = Asset("shaderFiles/fragmentShader2.txt", "shaderFiles/vertexShader2.txt", "resources/MarioSMBW.png");

	gAsset.drawType = GL_TRIANGLES;
	gAsset.drawStart = 0;
	gAsset.drawCount = 3;

	bytes_allocated = 0;

	//loadTriangle();
	setup();
	loadInstances();
	

	gAsset.textureLocation = glGetUniformLocation(gAsset.shaderBase.programHandle, "tex");
	double lastTime = glfwGetTime();
	double lastSecond = lastTime;
	double limitFPS = 1.0/60.0;
	while(!glfwWindowShouldClose(gWindow))
	{
		glfwPollEvents();
		
		// update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        double deltaTime = thisTime - lastTime;
        Update((float)(deltaTime));
        lastTime = thisTime;
        
        if(thisTime - lastSecond >= 1)
        {
        	lastSecond = thisTime;
        	std::cout << "fps: " << 1.0/deltaTime << std::endl;
        }

        //draw one frame
		Render();

		GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error "<< error << ": " << gluErrorString(error) << std::endl;

		//exit program if escape key is pressed
        if(glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gWindow, GL_TRUE);


        double sleepTime = limitFPS - (glfwGetTime() - thisTime);
        //std::cout << "limitFPS: " << limitFPS << " sleepTime: " << sleepTime << " deltaTime: " << deltaTime << std::endl;
        if(sleepTime > 0) std::this_thread::sleep_for(std::chrono::milliseconds((long)(sleepTime*1000)));
	}
	
	return 0;	
}

