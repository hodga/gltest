#include "renderEngine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <assert.h>

using namespace hwgl;
using namespace std;

RenderEngine::RenderEngine()
{
	initializeOpenGL();

	//create shader
	shaderBase.addShaderFromFile("shaderFiles/fragmentShader2.txt", GL_FRAGMENT_SHADER);
	shaderBase.addShaderFromFile("shaderFiles/vertexShader2.txt", GL_VERTEX_SHADER);
	shaderBase.makeProgram();
}

shared_ptr<Sprite> RenderEngine::makeSprite(std::string filePath, std::string batchName) 
{
	shared_ptr<Sprite> sprite(new Sprite());
	sprite->filename = filePath;

	if(!batchesToUpdate[batchName])
	{
		if(texturePackers.find(batchName) == texturePackers.end())
		{
			batchesToUpdate[batchName] = true;
			renderBatches[batchName] = RenderBatch();
		}

		for(auto &tpi : texturePackers)
		{
			if(tpi.second.getPackedImageData().find(filePath) == tpi.second.getPackedImageData().end())
			{
				cout << "need to repack texture!" << endl;
				batchesToUpdate[batchName] = true;
				break;
			}
		}

		if(!batchesToUpdate[batchName])
		{
			unsigned pW = texturePackers[batchName].getPackedWidth();
			unsigned pH = texturePackers[batchName].getPackedHeight();
			auto images = texturePackers[batchName].getPackedImageData();

			image_t image = images[sprite->filename];

			sprite->textureCell = glm::vec2((float)image.x/pW,(float)image.y/pH);
			sprite->textureSize = glm::vec2((float)image.width/pW,(float)image.height/pH);
			sprite->size = glm::vec2((float)image.width,(float)image.height);
		}
	}

	batchedSprites[batchName].push_back(sprite);
	
	return sprite;
}

void RenderEngine::step(const float deltaTime)
{
	cleanExpiredSprites();

	//update
	for(auto &rbi : renderBatches) 
	{
		string batchName = rbi.first;
		if(batchesToUpdate[batchName])
		{
			batchesToUpdate[batchName] = false;
			packTextureForBatch(batchName);
			setupBatch(batchName);
		}

		auto sprites = batchedSprites[batchName];

		if(rbi.second.instances.size() > sprites.size())
		{
			rbi.second.instances.erase(rbi.second.instances.begin()+sprites.size());
			assert(rbi.second.instances.size() == sprites.size());
		}

		for(int i = 0; i < batchedSprites[batchName].size();i++)
		{
			if(i >= rbi.second.instances.size()) 
			{
				rbi.second.instances.push_back(Instance());
			}

			//TODO use boolean value here to decide if sprite should be rendered
			sprites[i]->animate(deltaTime);

			rbi.second.instances[i].pos = sprites[i]->pos;
			rbi.second.instances[i].size = sprites[i]->size;
			rbi.second.instances[i].textureCell = sprites[i]->textureCell;
			rbi.second.instances[i].textureSize = sprites[i]->textureSize;

			//cout << "batch instance " << i << "texC (" << rbi.second.instances[i].textureSize.x << ":" << rbi.second.instances[i].textureSize.y << ")" << endl;
		}

		rbi.second.camera.pos = -cameraPos;
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

	window = glfwCreateWindow((int)800, (int)800, "OpenGL Test", NULL, NULL);

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

void RenderEngine::cleanExpiredSprites()
{
	for(auto &rbi : renderBatches) 
	{
		string batchName = rbi.first;
		for(auto sprite = batchedSprites[batchName].begin(); sprite != batchedSprites[batchName].end();sprite++)
		{
			if(sprite->unique())
			{
				cout << "single owner, removing sprite" << endl;
				sprite = batchedSprites[batchName].erase(sprite) - 1;
			}
		}
	}
}

void RenderEngine::packTextureForBatch(string batchName) 
{
	vector<shared_ptr<Sprite> > sprites = batchedSprites[batchName];
	texturePackers[batchName] = TexturePacker();
	for(auto sprite : sprites) 
	{
		string imagePath = sprite->filename;
		texturePackers[batchName].addImage(imagePath);
	}

	texturePackers[batchName].packImages(batchName);

	unsigned pW = texturePackers[batchName].getPackedWidth();
	unsigned pH = texturePackers[batchName].getPackedHeight();
	auto images = texturePackers[batchName].getPackedImageData();

	for(auto sprite : sprites) 
	{
		image_t image = images[sprite->filename];

		sprite->textureCell = glm::vec2((float)image.x/pW,(float)image.y/pH);
		sprite->textureSize = glm::vec2((float)image.width/pW,(float)image.height/pH);

		if(sprite->size.x == 0 && sprite->size.y == 0)
		{
			sprite->size = glm::vec2((float)image.width,(float)image.height);
		}
	}
}

void RenderEngine::setupBatch(string batchName)
{
	renderBatches[batchName] = RenderBatch(texturePackers[batchName].getPackedImage(), 
				   texturePackers[batchName].getPackedWidth(),
				   texturePackers[batchName].getPackedHeight());

	renderBatches[batchName].drawType = GL_TRIANGLES;
	renderBatches[batchName].drawStart = 0;
	renderBatches[batchName].drawCount = 3;
	renderBatches[batchName].setup(shaderBase);
}