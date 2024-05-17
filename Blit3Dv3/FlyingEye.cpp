#include "FlyingEye.h"


const int MIN_POS_Y = 105;
const int MAX_POS_Y = 1020;
const float MAX_VELOCITY = 8.1f;
const float FALL_GRAVITY = -30.f;
const float JUMP_GRAVITY = 50.f;

void FlyingEye::Draw() 
{
	if (dead) 
	{
		death[deathFrame]->Blit(position.x, position.y, scale, scale);
	}	
	else if (damaged)
	{
		damage[damageFrame]->Blit(position.x, position.y, scale, scale);
	}
	else {
		flight[flightFrame]->Blit(position.x, position.y, scale, scale);
	}
}

bool FlyingEye::Update(float seconds) 
{
	animationTimer += seconds;
	 
	if (!damaged && !dead && animationTimer >= frameSlice) {
		flightFrame++;
		if (flightFrame >= flight.size()) {
			flightFrame = 0;
		}
		animationTimer -= frameSlice;
	}

	if (damaged && !dead && animationTimer >= frameSlice) {
		position.x -= 70;
		damageFrame++;
		if (damageFrame >= damage.size()) {
			dead = true;
			damageFrame = 0;
		}
		animationTimer -= frameSlice;
	}

	if (dead && animationTimer >= frameSlice) {
		if (deathFrame < death.size() - 1) {
			if (position.y > MIN_POS_Y)
				deathFrame = 0;
			else
				deathFrame++;
		}
		animationTimer -= frameSlice;
	}

	// calculate velocites
	if (pulse) {
		velocity.y += JUMP_GRAVITY;
		pulse = false;
		flightFrame = 6;
	}

	// limit velocity
	if (velocity.y > MAX_VELOCITY)
		velocity.y = MAX_VELOCITY;

	position.y += velocity.y;
	// add acceleration
	velocity += FALL_GRAVITY * seconds;

	if (position.y <= MIN_POS_Y) {
		position.y = MIN_POS_Y;
		velocity -= FALL_GRAVITY * seconds;
	}

	if (position.y >= MAX_POS_Y)
		position.y = MAX_POS_Y;

	return true;
}

void FlyingEye::reset() {
	position = glm::vec2(1920.f / 2, 1080.f / 2);
	velocity = glm::vec2(0, 10);
	flightFrame = 0;
	deathFrame = 0;
	damageFrame = 0;
	damaged = false;
	dead = false;
	animationTimer = 0;
}