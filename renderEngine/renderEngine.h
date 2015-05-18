#pragma once

#import <vector>
#import <map>
#import <string>

#import "renderTrait.h"
#import "renderBatch.h"
#import "instance.h"
#import "textureLib/texturePacker.h"
#import "../testObject.h"

namespace hwgl 
{
	class RenderEngine
	{
	public:
		GLFWwindow* window;
		
		~RenderEngine() {}

		//template <typename Container> 
		void setup(std::vector<hwge::TestObject> &traits);
		//template <typename Container> 
		void update(std::vector<hwge::TestObject> &traits);

		//run one simulation step (how to handle asynchronous?)
		void step(const float deltaTime);

	private:
		

		std::map<std::string, TexturePacker> texturePackers;
		std::map<std::string, RenderBatch> renderBatches;

		void initializeOpenGL();
	};
}