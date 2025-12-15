#pragma once

#include "Shape3d.h"
#include "Material.h"

/* Sphere class */
class Sphere : public Shape3D
{
public:
	enum DIRECTION { NONE, LEFT, UP, RIGHT, DOWN };
	Sphere();
	Sphere(float r, int sl, int st);

	void setRadius(float r);
	float getRadius() const;
	void setSlice(int sl);
	void setStack(int st);

	void setIndexPosition(int x, int y);
	int getXIndex() const;
	int getYIndex() const;
	bool isIndexPositionUpdated();

	void setCurrentDirection(DIRECTION d);
	void setNextDirection(DIRECTION d);
	DIRECTION getCurrentDirection() const;
	DIRECTION getNextDirection() const;
	void updateDirection();

	void move();
	virtual void draw() const;
	void settime(int t);
	int gettime();

protected:
	float radius;
	int slice;
	int stack;
	int time;

	int idxPos[2];
	bool bInxPosUpdated;

	DIRECTION currDirection;
	DIRECTION nextDirection;

};
