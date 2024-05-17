#include "Ring.h"

extern float gameVelocity;
const int MAX_BUMP_TICKS = 20;

Ring::Ring(std::vector<Sprite*> sprites, float scale, glm::vec2 position) {
	this->position = position;
	this->spriteL = sprites[0];
	this->spriteR = sprites[1];
	this->scale = scale;
	this->height = 50.f * scale;					// 50.f obtained from file itself
	this->rHoopWidth = 19.f * scale;				// 19.f from file
	this->lHoopWidth = 18.f * scale;				// 18.f from file
	this->barThickness = 10.f * scale;				// for hitbox 
	this->bumpTicks = 0;
}

void Ring::DrawSpriteL() {
	spriteL->Blit(position.x - lHoopWidth / 2, position.y, scale, scale);
}

void Ring::DrawSpriteR() {
	spriteR->Blit(position.x + rHoopWidth / 2, position.y, scale, scale);
}

bool Ring::Update(float seconds) {
	if (position.x <= - rHoopWidth - lHoopWidth) {
		return false;
	}

	position.x += gameVelocity * seconds;

	if (bumped && bumpTicks <= MAX_BUMP_TICKS) {
		if (bumpTicks % 2 == 0)
		{
			position.y += 5;
			position.x += 2;
		}
		else
		{
			position.y -= 5;
			position.x += 2;
		}
		bumpTicks++;
	}
	 
	return true;
}

Ring RingFactory::generateRing(float seconds, int level) {
	rng.seed(rd());
	std::uniform_int_distribution<unsigned int> uint_y_pos(200, 800);
	Ring* resultRing = new Ring(ringSprites, 7.0, glm::vec2(2100, uint_y_pos(rng)));
	return *resultRing;
}