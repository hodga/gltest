#include "asset.h"

using namespace hwgl;

Asset::Asset() {}

Asset::Asset(std::string nFragmentShaderFile, std::string nVertexShaderFile, std::string nTextureFile)
{
	fragmentShaderFile = nFragmentShaderFile;
	vertexShaderFile = nVertexShaderFile;
	textureFile = nTextureFile;

	shaderBase.addShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER);
	shaderBase.addShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER);
	shaderBase.makeProgram();

	texture.reset( new Texture(textureFile));
}