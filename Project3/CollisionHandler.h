#pragma once

#include "CollisionDetector.h"
extern int frightTime;
extern int score;
extern int collision_counter;
extern bool init;
class CollisionHandler
{
public:
	void operator()(PacMan& pacman, Map& map);
	void operator()(PacMan& pacman, Ghost& ghost);
};
