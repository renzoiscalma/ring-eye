#include "CollisionCheck.h"

bool CollisionCheck::EyeRingCollide(FlyingEye flyingEye, Ring ring) {
	// rings have two hit boxes, one on top and one on bottom. check if it collides with hitbox of eye

	// compute the edges of eye hitbox
	float eHB_rightEdge = flyingEye.position.x + flyingEye.hitboxRadius + flyingEye.hitboxOffset.x;
	float eHB_leftEdge = flyingEye.position.x - flyingEye.hitboxRadius + +flyingEye.hitboxOffset.x;
	float eHB_topEdge = flyingEye.position.y + flyingEye.hitboxRadius;
	float eHB_bottomEdge = flyingEye.position.y - flyingEye.hitboxRadius;

	// compute the edges of the top hitbox of ring
	float rHBTop_rightEdge = ring.position.x + ring.barThickness / 2;
	float rHBTop_leftEdge = ring.position.x - ring.barThickness / 2;
	float rHBTop_topEdge = ring.position.y + (ring.height / 2);
	float rHBTop_bottomEdge = ring.position.y + (ring.height / 2) - ring.barThickness;

	// compute the edges of the bottom hitbox of ring
	float rHBBot_rightEdge = ring.position.x + ring.barThickness / 2;
	float rHBBot_leftEdge = ring.position.x - ring.barThickness / 2;
	float rHBBot_topEdge = ring.position.y - (ring.height / 2) + ring.barThickness;
	float rHBBot_bottomEdge = ring.position.y - (ring.height / 2);

	bool collidedTopHitbox = eHB_rightEdge >= rHBTop_leftEdge &&				// check eye hitbox collision with top ring hitbox
		eHB_leftEdge <= rHBTop_rightEdge &&
		eHB_bottomEdge <= rHBTop_topEdge &&
		eHB_topEdge >= rHBTop_bottomEdge;

	bool collidedBotHitbox = eHB_rightEdge >= rHBBot_leftEdge &&				// check eye hitbox collision with bottom ring hitbox
		eHB_leftEdge <= rHBBot_rightEdge &&
		eHB_bottomEdge <= rHBBot_topEdge &&
		eHB_topEdge >= rHBBot_bottomEdge;

	return (collidedTopHitbox || collidedBotHitbox) &&
		(!flyingEye.damaged || !flyingEye.dead);
}

bool CollisionCheck::EyeRingScore(FlyingEye flyingEye, Ring ring) {
	if (flyingEye.position.x > ring.position.x &&													// if ring position exceeds eye
		flyingEye.position.x < ring.position.x + ring.rHoopWidth &&									// eye position within the bounds of right hoop
		(flyingEye.position.y <= ring.position.y + ring.height/2 - ring.barThickness &&				// below the top part of hoop
		flyingEye.position.y >= ring.position.y - ring.height/2 + ring.barThickness) &&				// above the bottom part of hoop
		!ring.scored)				
	{
		return true;
	}
	return false;
}