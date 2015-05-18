#pragma once

#include <string>

#include "renderEngine/renderTrait.h"

using namespace hwgl;

namespace hwge 
{
	class TestObject : public RenderTrait
	{
	public:
		TestObject();
		TestObject(std::string nImagePath);

		virtual ~TestObject() {}

		virtual std::string getImagePath() override;

	private:
		std::string imagePath;
	};
}