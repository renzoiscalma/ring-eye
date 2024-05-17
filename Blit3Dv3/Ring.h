#pragma once
#include "Blit3D.h"
#include <random>
class Ring {
public:
	Ring(std::vector<Sprite*> sprites, float scale, glm::vec2 position);
	glm::vec2 position;
	Sprite* spriteL;
	Sprite* spriteR;
	int bumpTicks;
	float scale;				// scale of the ring used for draw
	float height;				// height of the ring
	float rHoopWidth;			// width of right hoop
	float lHoopWidth;			// width of left hoop
	float barThickness;			// for hitbox 
	bool scored = false;
	bool bumped = false;
	void DrawSpriteL();
	void DrawSpriteR();
	bool Update(float seconds);
};

class RingFactory {
public:
	std::vector<Sprite*> ringSprites;
	bool generate = false;
	float spawnTimer = 3.f;
	std::random_device rd;
	std::mt19937 rng;
	Ring generateRing(float seconds, int level); // generates ring depending on gameTime and level
};