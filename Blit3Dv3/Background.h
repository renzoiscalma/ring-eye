#pragma once
#include "Blit3D.h"
class Background 
{
public:
	Sprite *sprite;
	glm::vec2 position = glm::vec2(1920.f / 2, 1080.f / 2);
	void Draw();
	void Update(float seconds);
};