#include "Background.h"

extern float gameVelocity;
void Background::Draw() {
	// draw two pieces of background in order to "wrap" it
	sprite->Blit(position.x, position.y);
	sprite->Blit(position.x + 1920.f, position.y);
}

void Background::Update(float seconds) {
	position.x += gameVelocity * seconds;

	// snap the background back to initial place if it has scrolled off the screen
	if (position.x <= -1920/2)
		position.x = 1920.f/2;
}