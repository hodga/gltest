#pragma once

#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include "renderBatch.h"
#include "sprite.h"
#include "textureLib/texturePacker.h"

namespace hwgl 
{

	class RenderEngine
	{
	public:
		GLFWwindow* window;

		glm::vec2 cameraPos;
		float cameraZoom;
		
		RenderEngine();


		std::shared_ptr<Sprite> makeSprite(std::string filePath, std::string batchName = "-");

		//run one simulation step (how to handle asynchronous?)
		void step(const float deltaTime);	

	private:
		std::map<std::string, std::vector<std::shared_ptr<Sprite> > > batchedSprites;

		std::map<std::string, TexturePacker> texturePackers;
		std::map<std::string, RenderBatch> renderBatches;

		std::map<std::string, bool> batchesToUpdate; //hopefully I don't need this if the thing ever gets to "release mode"

		ShaderBase shaderBase;

		void initializeOpenGL();

		void cleanExpiredSprites();
		void setupBatch(std::string batchName);
		void packTextureForBatch(std::string batchName);
	};
}