#include "instance.h"

using namespace hwgl;

Instance::Instance(glm::vec2 pos, glm::vec2 inTextureCell, glm::vec2 inTextureSize)
{
	//transform = inTransform;
	//textureCoordinate = inTextureCoordinate;

	data[0] = pos[0];
	data[1] = pos[1];

	data[2] = inTextureCell[0];
	data[3] = inTextureCell[1];

	data[4] = inTextureSize[0];
	data[5] = inTextureSize[1];

	//transformLocation = glGetUniformLocation(asset->shaderBase.programHandle, "modelPosition");
}

/*void Instance::Render()
{
	

	//set shader uniforms
    glUniformMatrix4fv(transformLocation, 1, false, glm::value_ptr(transform));
    

    //bind textures
    
	

    //bind VAO and draw
    
	glDrawArrays(asset->drawType, asset->drawStart, asset->drawCount);

    
}*/