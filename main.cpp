
#include "gameEngine/GameEngine.h"

using namespace hwge;

GameEngine gameEngine;


/*static void Update(float secondsElapsed) {

    //move position of camera based on WASD keys, and XZ keys for up and down
   const float moveSpeed = 800; //units per second
    if(glfwGetKey(gRenderEngine.window, 'S'))
    {
    	gHero.move(glm::vec2(0,-1));
        //gSprites[0]->pos += glm::vec2(0,-moveSpeed*secondsElapsed);
    } 
    else if(glfwGetKey(gRenderEngine.window, 'W'))
    {
    	gHero.move(glm::vec2(0,1));
        //gSprites[0]->pos += glm::vec2(0,moveSpeed*secondsElapsed);
    }
    if(glfwGetKey(gRenderEngine.window, 'A'))
    {
    	gHero.move(glm::vec2(-1,0));
        //gSprites[0]->pos += glm::vec2(-moveSpeed*secondsElapsed, 0);
    } 
    else if(glfwGetKey(gRenderEngine.window, 'D'))
    {
    	gHero.move(glm::vec2(1,0));
        //gSprites[0]->pos += glm::vec2(moveSpeed*secondsElapsed, 0);
    }


    if(gRenderEngine.cameraPos.x > gSprites[0]->pos.x+600) gRenderEngine.cameraPos.x = gSprites[0]->pos.x+600;
    else if(gRenderEngine.cameraPos.x < gSprites[0]->pos.x-600) gRenderEngine.cameraPos.x = gSprites[0]->pos.x-600;
    if(gRenderEngine.cameraPos.y > gSprites[0]->pos.y+600) gRenderEngine.cameraPos.y = gSprites[0]->pos.y+600;
    else if(gRenderEngine.cameraPos.y < gSprites[0]->pos.y-600) gRenderEngine.cameraPos.y = gSprites[0]->pos.y-600;

}*/

int main(int argc, char*argv[])
{
	gameEngine.run();
	
	return 0;	
}

