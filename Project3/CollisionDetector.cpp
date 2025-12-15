#include "CollisionDetector.h"
#include "Sphere.h"
#include "Map.h"
#include<iostream>
using namespace std;

bool CollisionDetector::operator()(const PacMan& pacman, Map& map) {
    
    int i = pacman.getXIndex();
    int j = pacman.getYIndex();
    switch (pacman.getCurrentDirection()) {
    case Sphere::DIRECTION::UP:
        return !map.getBlock(i, j + 1).isPassable();
    case Sphere::DIRECTION::RIGHT:
        return !map.getBlock(i + 1, j).isPassable();
    case Sphere::DIRECTION::DOWN:
        if ((pacman.getYIndex() == 21 && pacman.getXIndex() == 13) || (pacman.getYIndex() == 21 && pacman.getXIndex() == 14)) {
            return true;
        }
        else return !map.getBlock(i, j - 1).isPassable();
    case Sphere::DIRECTION::LEFT:
        return !map.getBlock(i - 1, j).isPassable();
    default:
        return false;
    }
}
bool CollisionDetector::operator()(const PacMan& pacman, const Ghost& ghost) {
    Vector3f pac_pos = pacman.getCenter();
    Vector3f ghost_pos = ghost.getCenter();
    if (dotProduct(pac_pos - ghost_pos, pac_pos - ghost_pos) <= (pacman.getRadius() + ghost.getRadius()) * (pacman.getRadius() + ghost.getRadius())) {
        return true;
    }
    else
        return false;
}