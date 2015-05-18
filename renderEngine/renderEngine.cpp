#import "renderEngine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <assert.h>

using namespace hwgl;
using namespace std;

void RenderEngine::setup(vector<hwge::TestObject> &traits)
{
	assert(!traits.empty());

	initializeOpenGL();

	//prepare images for packing
	for(auto &renderTrait : traits) 
	{
		string batchName = renderTrait.getBatchName();
		string imagePath = renderTrait.getImagePath();

		texturePackers[batchName].addImage(imagePath);
	}

	//create shader
	ShaderBase shaderBase;
	shaderBase.addShaderFromFile("shaderFiles/fragmentShader2.txt", GL_FRAGMENT_SHADER);
	shaderBase.addShaderFromFile("shaderFiles/vertexShader2.txt", GL_VERTEX_SHADER);
	shaderBase.makeProgram();


	//pack textures, shaders and instances into render batches
	for(auto &tpi: texturePackers) 
	{
		tpi.second.packImages();

		renderBatches[tpi.first] = RenderBatch(tpi.second.getPackedImage(), 
				   tpi.second.getPackedWidth(),
				   tpi.second.getPackedHeight());

		renderBatches[tpi.first].drawType = GL_TRIANGLES;
		renderBatches[tpi.first].drawStart = 0;
		renderBatches[tpi.first].drawCount = 3;
		renderBatches[tpi.first].setup(shaderBase);
	}

	map<string, vector<hwge::TestObject *> > traitsVectorMap;
	for(auto &renderTrait : traits) 
	{
		string batchName = renderTrait.getBatchName();
		unsigned pW = texturePackers[batchName].getPackedWidth();
		unsigned pH = texturePackers[batchName].getPackedHeight();
		auto images = texturePackers[batchName].getPackedImageData();

		image_t image = images[renderTrait.getImagePath()];

		glm::vec2 texCellCoord = glm::vec2((float)image.x/pW,(float)image.y/pH);
		glm::vec2 texSize = glm::vec2((float)image.width/pW,(float)image.height/pH);
		glm::mat4 transform = glm::translate(glm::mat4(), glm::vec3(-0.5,-0.5,0)) * glm::scale(glm::mat4(), glm::vec3((float)image.width/pW,(float)image.height/pH, 1));
		//glm::mat4() * glm::scale(glm::mat4(), glm::vec3((float)image.width/pW,(float)image.height/pH, 1));

		Instance instance(texCellCoord, texSize, transform);

		renderBatches[batchName].instances.push_back(instance);

		traitsVectorMap[batchName].push_back(&renderTrait);
	}

	//TODO: how to do this in a more safe manner?
	for(auto &rbi : renderBatches)
	{
		for(int i = 0; i < traitsVectorMap[rbi.first].size();i++)
		{
			traitsVectorMap[rbi.first][i]->setTransformPointer(&rbi.second.instances[i].transform);
		}
	}
}

void RenderEngine::update(vector<hwge::TestObject> &traits)
{
	throw runtime_error("RenderEngine::update() not implemented.");
}

void RenderEngine::step(const float deltaTime)
{
	//update
	for(auto &rbi: renderBatches) 
	{
		rbi.second.update();
	}

	//render
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	for(auto &rbi: renderBatches) 
	{
		glUseProgram(rbi.second.shaderBase.programHandle);
		rbi.second.render();
	}

	glUseProgram(0);

	glfwSwapBuffers(window);
}


void onError(int errorCode, const char *msg)
{
	throw runtime_error(msg);
}

void RenderEngine::initializeOpenGL()
{
	glfwSetErrorCallback(onError);
	if(!glfwInit())
	{
		throw runtime_error("Error initializing glfw.");
	}

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow((int)800, (int)600, "OpenGL Test", NULL, NULL);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if(!window) 
	{
		throw runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
	}
	
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		throw runtime_error("glewInit failed");
	}

	// print out some info about the graphics drivers
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	
	if(!GLEW_VERSION_3_2)
	{
		throw runtime_error("OpenGL 3.2 API is not available");
	}
}