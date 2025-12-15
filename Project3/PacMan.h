#pragma once
#include "Sphere.h"
#include "TextureHandler.h"
#include <vector>

class PacMan : public Sphere
{
public:
	PacMan(float r, int sl, int st, bool bCol, int l);

	void setCollided(bool bCol);
	virtual void draw() const;
	void setghost_time(int t);
	int getghost_time();
	int getlife();
	void setlife(int l);
	

	void loadTextures(const std::vector<std::string>& textureFiles);
private:
	bool bCollided;
	int life;
	int ghosttime;

	GLuint textureIDs[16];
};
