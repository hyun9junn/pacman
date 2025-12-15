#pragma once

#include "Sphere.h"
#include "Map.h"
#include "PacMan.h"
#include "Ghost.h"

class CollisionDetector
{
public:
	bool operator()(const PacMan& pacman, Map& map);
	bool operator()(const PacMan& pacman, const Ghost& ghost);
};