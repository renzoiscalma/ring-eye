#pragma once
#include "Blit3D.h"
class FlyingEye {
public:
	std::vector<Sprite*> flight;
	std::vector<Sprite*> death;
	std::vector<Sprite*> damage;
	glm::vec2 position = glm::vec2(1920.f / 2, 1080.f / 2);
	glm::vec2 velocity;
	int flightFrame = 0;
	int deathFrame = 0;
	int damageFrame = 0;
	float angle = 0;
	float frameSlice = 1.f / 6.f;
	float animationTimer = 0.f;
	float scale = 4.f;
	float hitboxRadius = 20.f;
	glm::vec2 hitboxOffset = glm::vec2(20, 0);
	bool dead = false;
	bool damaged = false;
	bool pulse = false;
	bool Update(float seconds);
	void Draw();
	void reset();
};