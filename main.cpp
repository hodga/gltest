#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>


#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <thread>
#include <chrono>
#include <array>

/*
#include "instance.h"
#include "textureLib/texturePacker.h"
#include "renderBatch.h"*/

#include "renderEngine/renderEngine.h"
#include "testObject.h"

using namespace hwgl;

// globals

TexturePacker gTexturePacker;

RenderBatch gRenderBatch;

std::vector<Instance> gInstances;

Instance *instance;

size_t bytes_allocated;

RenderEngine gRenderEngine;

std::vector<hwge::TestObject> testObjects;

/*void onError(int errorCode, const char *msg)
{
	throw std::runtime_error(msg);
}

void loadInstances()
{
	auto images = gTexturePacker.getPackedImageData();
	unsigned pW = gTexturePacker.getPackedWidth();
	unsigned pH = gTexturePacker.getPackedHeight();
	int x = 0;
	glm::vec3 pos[] = {glm::vec3(-0.5,-0.5,0),
				glm::vec3(0.5,0.5,0),
				glm::vec3(0.5,-0.5,0),
				glm::vec3(-0.5,0.5,0),
				glm::vec3(0,0,0),
				glm::vec3(-0.5,0,0),
				glm::vec3(0,-0.5,0),
				glm::vec3(0.5,0,0),
				glm::vec3(0,0.5,0)};
	for(auto image : images)
	{
		cout << "image: (" << image.x << "," << image.y << ") dim: (" << image.width << "," << image.height << ")" << endl;
		glm::vec2 texCellCoord = glm::vec2((float)image.x/pW,(float)image.y/pH);
		glm::vec2 texSize = glm::vec2((float)image.width/pW,(float)image.height/pH);

		glm::mat4 transform = glm::translate(glm::mat4(), pos[x]) * glm::scale(glm::mat4(), glm::vec3((float)image.width/pW,(float)image.height/pH, 1));

		Instance instance(texCellCoord, texSize, transform);
		gInstances.push_back(instance);
		x++;
	}
}

void Render()
{
	gRenderBatch.update();

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(gRenderBatch.shaderBase.programHandle);

	gRenderBatch.render();

	glUseProgram(0);

	glfwSwapBuffers(gWindow);
}
*/
static void Update(float secondsElapsed) {

    //move position of camera based on WASD keys, and XZ keys for up and down
   const float moveSpeed = 2; //units per second
    if(glfwGetKey(gRenderEngine.window, 'S'))
    {
        *testObjects[0].transform *= glm::translate(glm::mat4(), glm::vec3(0,-moveSpeed*secondsElapsed,0));
    } 
    else if(glfwGetKey(gRenderEngine.window, 'W'))
    {
        *testObjects[0].transform *= glm::translate(glm::mat4(), glm::vec3(0,moveSpeed*secondsElapsed,0));
    }
    if(glfwGetKey(gRenderEngine.window, 'A'))
    {
        *testObjects[0].transform *= glm::translate(glm::mat4(), glm::vec3(-moveSpeed*secondsElapsed,0,0));
    } 
    else if(glfwGetKey(gRenderEngine.window, 'D'))
    {
        *testObjects[0].transform *= glm::translate(glm::mat4(), glm::vec3(moveSpeed*secondsElapsed,0,0));
    }
}

int main(int argc, char*argv[])
{
	/*glfwSetErrorCallback(onError);
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

	//build shader
	ShaderBase shaderBase;
	shaderBase.addShaderFromFile("shaderFiles/fragmentShader2.txt", GL_FRAGMENT_SHADER);
	shaderBase.addShaderFromFile("shaderFiles/vertexShader2.txt", GL_VERTEX_SHADER);
	shaderBase.makeProgram();


	//pack textures
	gTexturePacker.addImage("resources/MarioSMBW.png");
	gTexturePacker.addImage("resources/Bowser_-_New_Super_Mario_Bros_2.png");
	gTexturePacker.addImage("resources/hazard.png");
	gTexturePacker.addImage("resources/LuigiNSMBW.png");
	gTexturePacker.addImage("resources/toad.png");
	gTexturePacker.addImage("resources/150px-MLSS_-_Princess_Peach_Artwork.png");
	//gTexturePacker.addImage("resources/Mario_SM64DS.png");
	//gTexturePacker.addImage("resources/Mushroom_smas.png");
	//gTexturePacker.addImage("resources/PMTTYDGreenBoots.png");
	gTexturePacker.packImages();

	//load instances (using texture)
	loadInstances();

	//create render batch
	gRenderBatch = RenderBatch(gTexturePacker.getPackedImage(), 
				   gTexturePacker.getPackedWidth(),
				   gTexturePacker.getPackedHeight());

	gRenderBatch.drawType = GL_TRIANGLES;
	gRenderBatch.drawStart = 0;
	gRenderBatch.drawCount = 3;

	gRenderBatch.setup(shaderBase);
	gRenderBatch.instances = gInstances;


	instance = &(gRenderBatch.instances.back());*/

	hwge::TestObject testObject("resources/MarioSMBW.png");
	hwge::TestObject testObject2("resources/LuigiNSMBW.png");

	testObjects.push_back(testObject);
	testObjects.push_back(testObject2);

	gRenderEngine.setup(testObjects);

	//start rendering
	double lastTime = glfwGetTime();
	double lastSecond = lastTime;
	double limitFPS = 1.0/60.0;
	

	while(!glfwWindowShouldClose(gRenderEngine.window))
	{
		//cout << "while loop step" << glm::to_string(testObjects[0].transform) << endl;
		glfwPollEvents();

		/**testObjects[0].transform *= glm::translate(glm::mat4(), glm::vec3(0.1,0,0));
		*testObjects[1].transform *= glm::translate(glm::mat4(), glm::vec3(0.1,0,0));

		cout << "transform: " << testObjects[0].transform << endl;
		cout << "transform2: " << testObjects[1].transform << endl;*/

		*testObjects[1].transform *= glm::translate(glm::mat4(), glm::vec3(0.01,0,0));
		
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
		//Render();
		cout << "going to step" << endl;
        gRenderEngine.step((float)deltaTime);

		GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error "<< error << ": " << gluErrorString(error) << std::endl;

		//exit program if escape key is pressed
        if(glfwGetKey(gRenderEngine.window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gRenderEngine.window, GL_TRUE);


        double sleepTime = limitFPS - (glfwGetTime() - thisTime);
        //std::cout << "limitFPS: " << limitFPS << " sleepTime: " << sleepTime << " deltaTime: " << deltaTime << std::endl;
        if(sleepTime > 0) std::this_thread::sleep_for(std::chrono::milliseconds((long)(sleepTime*1000)));
	}
	
	return 0;	
}

