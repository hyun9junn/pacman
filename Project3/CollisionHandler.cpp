#include "CollisionHandler.h"
#include "CollisionDetector.h"
#include<iostream>
using namespace std;

CollisionDetector colDetector;

int collision_counter;

void CollisionHandler::operator()(PacMan& pacman, Map& map) {
    if (colDetector(pacman, map)) {
        pacman.setVelocity(0.0f, 0.0f, 0.0f);
        pacman.setCurrentDirection(Sphere::DIRECTION::NONE);
    }
}
void CollisionHandler::operator()(PacMan& pacman, Ghost& ghost) {
    if (colDetector(pacman, ghost)) {
        if (ghost.getState() == Ghost::FRIGHTENED) {
            ghost.setState(Ghost::EATEN);
            ghost.setfright_time(0);
            score += 300;
        }
        else if (ghost.getState() == Ghost::EATEN) {
        }
        else {
            pacman.setlife(pacman.getlife() - 1);
            init = true;
        }
    }
}

