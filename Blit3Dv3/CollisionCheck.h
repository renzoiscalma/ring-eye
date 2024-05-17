#pragma once
#include "Ring.h"
#include "FlyingEye.h"
#include "Blit3D.h"

class CollisionCheck {
public:
	bool EyeRingCollide(FlyingEye flyingEye, Ring ring);
	bool EyeRingScore(FlyingEye flyingEye, Ring ring);
};